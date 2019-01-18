#include "model.hpp"

#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#include <vtkOBJReader.h>
#include <vtkSTLReader.h>
#include <QDebug>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkProperty.h>
#include <vtkDICOMImageReader.h>
#include <QFileInfo>
#include <vtkImageProperty.h>
#include <vtkOBJReader.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageActor.h>
#include <vtkImageReader2.h>
#include <vtkImageData.h>
#include <vtkVolume.h>
#include <vtkNamedColors.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkStructuredPointsReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkInteractorObserver.h>

#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkInteractorStyleImage.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include "modelmanager.hpp"

Model::Model(QString fn, modelManager *manager, QObject *parent): QObject(parent),filename(fn)
{
    this->manager = manager;
}

Model::~Model()
{
    qDebug() << filename <<"removed";
}

QString Model::source() const
{
    return filename;
}

void Model::load()
{
    QFileInfo info(filename);
    qDebug() << "******************: "<< filename;
    int result = 0;
    if(info.isDir()){
        result = loadVolume();
        if(result){
            qDebug() << "failed. unknown directory for volume rendering: "<< filename;
        }
    } else if(info.isFile()){
        result = loadImage() && loadMesh();
        if( result ) {
            qDebug() << "failed. unknown file: "<< filename;
        }
    } else {
        loadPrimitive();
    }

    done(filename,result);
}

int Model::loadPrimitive()
{
    if(filename=="axes"){
        // Axes
        vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
        double axes_length = 20.0;
        int16_t axes_label_font_size = 20;
        axes->SetTotalLength(axes_length, axes_length, axes_length);
        axes->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
        axes->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
        axes->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
        axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(axes_label_font_size);
        axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(axes_label_font_size);
        axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(axes_label_font_size);
        axes->SetPickable(true);
        manager->getRenderer()->AddActor(axes);
        prop = axes;
        manager->insertModel(prop.Get(),this);
    }
    return 0;
}

int Model::loadVolume()
{
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

    // Create the reader for the data
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName(filename.toStdString().c_str());
    reader->Update();

    if(reader){
        // Create transfer mapping scalar value to opacity
        vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction =
                vtkSmartPointer<vtkPiecewiseFunction>::New();
        opacityTransferFunction->AddPoint(0,    0.00);
        opacityTransferFunction->AddPoint(500,  0.15);
        opacityTransferFunction->AddPoint(1000, 0.15);
        opacityTransferFunction->AddPoint(1150, 0.85);

        // Create transfer mapping scalar value to color
        vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
                vtkSmartPointer<vtkColorTransferFunction>::New();
        colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
        colorTransferFunction->AddRGBPoint(64.0, 1.0, 0.0, 0.0);
        colorTransferFunction->AddRGBPoint(128.0, 0.0, 0.0, 1.0);
        colorTransferFunction->AddRGBPoint(192.0, 0.0, 0.5, 0.0);
        colorTransferFunction->AddRGBPoint(255.0, 0.0, 0.2, 0.0);

        vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
          vtkSmartPointer<vtkPiecewiseFunction>::New();
        volumeGradientOpacity->AddPoint(0,   0.0);
        volumeGradientOpacity->AddPoint(90,  0.5);
        volumeGradientOpacity->AddPoint(100, 1.0);

        // The property describes how the data will look
        vtkSmartPointer<vtkVolumeProperty> volumeProperty =
                vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->SetColor(colorTransferFunction);
        volumeProperty->SetScalarOpacity(opacityTransferFunction);
        volumeProperty->ShadeOn();
        volumeProperty->SetInterpolationTypeToLinear();
        volumeProperty->SetGradientOpacity(volumeGradientOpacity);
        volumeProperty->SetAmbient(0.4);
        volumeProperty->SetDiffuse(0.6);
        volumeProperty->SetSpecular(0.2);

        // The mapper / ray cast function know how to render the data
        vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
                vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
        volumeMapper->SetInputConnection(reader->GetOutputPort());

        // The volume holds the mapper and the property and
        // can be used to position/orient the volume
        vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(volumeMapper);
        volume->SetProperty(volumeProperty);
        volume->SetPickable(true);
        manager->getRenderer()->AddVolume(volume);
        prop = volume;
        manager->insertModel(prop.Get(),this);


        vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
    //    imageSliceMapper->SetInputData(colorImage);
        imageSliceMapper->SetInputConnection(reader->GetOutputPort());
        vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();

        imageSlice->SetMapper(imageSliceMapper);
        manager->getRenderer()->AddViewProp(imageSlice);
//        vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
//        manager->getRenderer()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);


//        vtkSmartPointer<vtkImageActor> actor_img = vtkSmartPointer<vtkImageActor>::New();
//        actor_img->SetInputData(imageSlice);
//        actor_img->SetOpacity(.5);
//        actor_img->SetPickable(true);
//        int *a =input->GetDimensions();
//        qDebug() << a[0] << a[1];
//        //          actor_img->SetOrigin(a[0]/2,a[1]/2,0);
//        actor_img->SetPosition(-a[0]/2,-a[1]/2,0);
//        manager->getRenderer()->AddActor(actor_img);

        return 0;
    } else {
        return -1;
    }

}

int Model::loadImage()
{
    QFileInfo info(filename);
    std::string stdstr = filename.toStdString();

    vtkSmartPointer<vtkAlgorithm> reader = nullptr;
    vtkSmartPointer<vtkImageData> input = nullptr;

    if(info.suffix()=="dcm"){
        vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkDICOMImageReader::New();
        //        dicomReader->SetDirectoryName(filename.c_str());
        dicomReader->SetFileName(stdstr.c_str());
        dicomReader->Update();
        input = dicomReader->GetOutput();
        reader = dicomReader;
    } else {
        vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
        vtkSmartPointer<vtkImageReader2> imageReader = readerFactory->CreateImageReader2(stdstr.c_str());
        if(imageReader){
            imageReader->SetFileName(stdstr.c_str());
            imageReader->Update();
            input = imageReader->GetOutput();
            reader = imageReader;
        }
    }

    if(reader){
        // Create an actor
        vtkSmartPointer<vtkImageActor> actor_img = vtkSmartPointer<vtkImageActor>::New();
        actor_img->SetInputData(input);
        actor_img->SetOpacity(.5);
        actor_img->SetPickable(true);
        int *a =input->GetDimensions();
        qDebug() << a[0] << a[1];
        //          actor_img->SetOrigin(a[0]/2,a[1]/2,0);
        actor_img->SetPosition(-a[0]/2,-a[1]/2,0);

        prop = actor_img;
        manager->insertModel(prop.Get(),this);
        manager->getRenderer()->AddActor(actor_img);
        return 0;
    } else {
        return -1;
    }
}

int Model::loadMesh()
{
    QFileInfo info(filename);
    std::string stdstr = filename.toStdString();

    auto objReader = vtkSmartPointer<vtkOBJReader>::New();
    auto stlReader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkPolyData> inputData = nullptr;

    std::string ext = filename.split('.').last().toStdString();
    qDebug() <<ext.c_str();
    if (ext == "obj") {
        // Read OBJ file
        objReader->SetFileName(stdstr.c_str());
        objReader->Update();
        inputData = objReader->GetOutput();
    } else if (ext == "stl") {
        // Read STL file
        stlReader->SetFileName(stdstr.c_str());
        stlReader->Update();
        inputData = stlReader->GetOutput();
    }

    if(inputData){
        auto translation = vtkSmartPointer<vtkTransform>::New();
        double n=500;
        translation->Scale(n,n,n);

        auto m_modelFilterTranslate = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        m_modelFilterTranslate->SetInputData(inputData);
        m_modelFilterTranslate->SetTransform(translation);
        m_modelFilterTranslate->Update();

        // Model Mapper
        vtkSmartPointer<vtkPolyDataMapper> m_modelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        m_modelMapper->SetInputConnection(m_modelFilterTranslate->GetOutputPort());
        m_modelMapper->ScalarVisibilityOff();

        // Model Actor
        auto m_modelActor = vtkSmartPointer<vtkActor>::New();
        m_modelActor->SetMapper(m_modelMapper);
        m_modelActor->GetProperty()->SetInterpolationToFlat();

        m_modelActor->GetProperty()->SetAmbient(0.1);
        m_modelActor->GetProperty()->SetDiffuse(0.7);
        m_modelActor->GetProperty()->SetSpecular(0.3);
        m_modelActor->GetProperty()->SetOpacity(0.6);
        m_modelActor->SetPosition(0.0, 0.0, 0.0);
        m_modelActor->SetPickable(true);
        prop = m_modelActor;
        manager->insertModel(prop.Get(),this);
        manager->getRenderer()->AddActor(m_modelActor);

        return 0;
    } else {
        return -1;
    }
}
