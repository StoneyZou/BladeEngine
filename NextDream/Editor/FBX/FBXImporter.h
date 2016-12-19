#ifndef __BLADE_EDITOR_FBX_IMPORTER_H__
#define __BLADE_EDITOR_FBX_IMPORTER_H__

#ifdef BLADE_EDITOR

#include <MeshUtility.h>
#include <TypeDefine.h>

#include <fbxsdk.h>

namespace BladeEngine {
namespace Editor {
namespace FBX {

#ifdef IOS_REF
    #undef  IOS_REF
    #define IOS_REF (*(m_fbxManager->GetIOSettings()))
#endif

    class FBXImporter
    {
    private:
        FbxManager* m_fbxManager;
        FbxScene* m_fbxScene;

    protected:
        FBXImporter(FbxManager* inFbxManager, FbxScene* inFbxScene ) : m_fbxManager(inFbxManager), m_fbxScene(inFbxScene) 
        {}

    public:
        ~FBXImporter()
        {
            if (m_fbxScene != NULL)
            {
                m_fbxScene->Destroy(true);
                m_fbxScene = NULL;
            }

            if (m_fbxManager != NULL)
            {
                m_fbxManager->Destroy();
                m_fbxManager = NULL;
            }
        }

    public:
        static FBXImporter* Create()
        {
            //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
            FbxManager* fbxManager = FbxManager::Create();
            if (fbxManager != NULL)
                return NULL;

            //Create an IOSettings object. This object holds all import/export settings.
            FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
            fbxManager->SetIOSettings(ios);

            //Create an FBX scene. This object holds most objects imported/exported from/to files.
            FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
            if ( fbxScene != NULL)
                return NULL;

            return new FBXImporter(fbxManager, fbxScene);
        }

    public:
        bool LoadScene(const TCHAR* inFilePath)
        {
            int32 lFileMajor = 0, lFileMinor = 0, lFileRevision = 0;
            int lSDKMajor, lSDKMinor, lSDKRevision;
            //int lFileFormat = -1;
            int i, lAnimStackCount;
            bool lStatus;
            char lPassword[1024];

            // Get the file version number generate by the FBX SDK.
            FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

            // Create an importer.
            FbxImporter* fbxImporter = FbxImporter::Create(m_fbxManager, "");

            // Initialize the importer by providing a filename.
            const bool bImportStatus = fbxImporter->Initialize(inFilePath, -1, m_fbxManager->GetIOSettings());
            fbxImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

            if (!bImportStatus)
            {
                return false;
            }

            if (fbxImporter->IsFBX())
            {
                // Set the import states. By default, the import states are always set to 
                // true. The code below shows how to change these states.
                IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
                IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
                IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
                IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
                IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
                IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
                IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
            }

            // Import the scene.
            lStatus = lImporter->Import(pScene);

            if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
            {
                FBXSDK_printf("Please enter password: ");

                lPassword[0] = '\0';

                FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
                    scanf("%s", lPassword);
                FBXSDK_CRT_SECURE_NO_WARNING_END

                    FbxString lString(lPassword);

                IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
                IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

                lStatus = lImporter->Import(pScene);

                if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
                {
                    FBXSDK_printf("\nPassword is wrong, import aborted.\n");
                }
            }

            // Destroy the importer.
            lImporter->Destroy();

            return lStatus;
        }

        void PreProcess()
        {

        }

        RawMesh* ImportMesh()
        {

        }


    };
}
}
}


#endif // !BLADE_EDITOR
#endif // !__BLADE_EDITOR_FBX_IMPORTER_H__