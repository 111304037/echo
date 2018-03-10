//#include "FbxImporter.h"
//#include <engine/core/Base/EchoDef.h>
//#include <engine/core/Math/EchoMath.h>
//#include <engine/core/Util/LogManager.h>
//#include <engine/core/Util/PathUtil.h>
//#include <engine/core/FileIO/ResourceGroupManager.h>
//
//#ifdef ECHO_PLATFORM_WINDOWS
//#include <fbxsdk.h>
//#endif
//
//namespace Echo{
//namespace Fbx
//{
//#ifdef ECHO_PLATFORM_WINDOWS
//	// ģ�ͽڵ� 
//	struct MeshNode
//	{
//		FbxNode*			m_node;
//		FbxNodeAttribute*	m_attrib;
//	};
//
//	// �ݹ��ȡ����ģ�ͽڵ�
//	static void recursiveGetAllMeshAtt(std::vector<MeshNode>& oMeshs, FbxNode* node)
//	{
//		for (int i = 0; i < node->GetNodeAttributeCount(); i++)
//		{
//			FbxNodeAttribute* attrib = node->GetNodeAttributeByIndex(i);
//			if (attrib->GetAttributeType() == FbxNodeAttribute::eMesh)
//			{
//				MeshNode mNode;
//				mNode.m_node = node;
//				mNode.m_attrib = attrib;
//				oMeshs.push_back(mNode);
//			}
//		}
//
//		for (int i = 0; i < node->GetChildCount(); i++)
//		{
//			FbxNode* cNode = node->GetChild(i);
//			recursiveGetAllMeshAtt(oMeshs, cNode);
//		}
//	}
//
//	/**
//	* ��Fbx�ļ��ж�ȡ�ڵ��޳�����
//	*/
//	bool importOccluderData(const char* fileName, vector<OccVertexInput>::type& oVertices, vector<Word>::type& oIndices)
//	{
//		oVertices.clear();
//		oIndices.clear();
//
//		String fbxPath = PathUtil::IsAbsolutePath(fileName) ? fileName : Echo::ResourceGroupManager::instance()->getFileLocation(fileName);
//
//		// ����ʵ��
//		FbxManager* lSdkManager = FbxManager::Create();
//
//		// Importing the Contents of an FBX File
//		FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
//		lSdkManager->SetIOSettings(ios);
//
//		// Create an importer using the SDK manager
//		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
//
//		// Use the first argument as the filename for the importer
//		if (!lImporter->Initialize(fbxPath.c_str(), -1, lSdkManager->GetIOSettings()))
//		{
//			EchoLogError("Importer FBX [%s] failed", fbxPath);
//			return false;
//		}
//
//		// Create a new scene so that it can be populated by the imported file
//		FbxScene* lScene = FbxScene::Create(lSdkManager, "MyScene");
//
//		// Import the contents of the file into the scene
//		lImporter->Import(lScene);
//
//		// The file is imported, so get rid of the importer
//		lImporter->Destroy();
//
//		// Access data form the fbx scene
//		FbxNode* lRootNode = lScene->GetRootNode();
//
//		// ���
//		int	vertOffset = 0;
//
//		// Recursive get all mehs attributes
//		std::vector<MeshNode> meshNodes;
//		recursiveGetAllMeshAtt(meshNodes, lRootNode);
//		for (size_t i = 0; i < meshNodes.size(); i++)
//		{
//			const MeshNode& meshNode = meshNodes[i];
//			FbxMesh* mesh = meshNode.m_node->GetMesh();
//			if (mesh)
//			{
//				FbxAMatrix globalTransform = meshNode.m_node->EvaluateGlobalTransform();
//
//				// �Ѽ���������
//				int polygonCount = mesh->GetPolygonCount();
//				for (int polyIdx = 0; polyIdx < polygonCount; polyIdx++)
//				{
//					int numVertices = mesh->GetPolygonSize(polyIdx);
//					for (int vertIdx = 0; vertIdx < numVertices; vertIdx++)
//					{
//						int globalVertIdx = mesh->GetPolygonVertex(polyIdx, vertIdx);
//						oIndices.push_back(globalVertIdx+vertOffset);
//					}
//				}
//
//				// ��������
//				int vertexCount = mesh->GetControlPointsCount();
//				for (int vertIdx = 0; vertIdx < vertexCount; vertIdx++)
//				{
//					FbxVector4 oriPosition = mesh->GetControlPointAt(vertIdx);
//					FbxVector4 position = globalTransform.MultT(oriPosition);
//					OccVertexInput drvi;
//					drvi.m_position = Echo::Vector3((float)position[0], (float)position[1], (float)position[2]);
//
//					oVertices.push_back( drvi);
//				}
//				vertOffset += vertexCount;
//			}
//			else
//			{
//				EchoLogError("Import fbx format to mesh failed...");
//			}
//		}
//
//		// ���ٳ���
//		lScene->Destroy();
//
//		// ����FBX������
//		lSdkManager->Destroy();
//
//		return true;
//	}
//
//	/**
//	* ת��Fbx��ʽ�ڵ��޳����ݵ�������
//	*/
//	bool convertOccluderDataToBinary(const char* destPathFileName, const char* origPathFileName)
//	{
//		vector<OccVertexInput>::type oVertices;
//		vector<Word>::type oIndices;
//
//		importOccluderData( origPathFileName, oVertices, oIndices);
//
//		FILE* fileHandle = fopen( destPathFileName, "wb");
//		if( fileHandle)
//		{
//			int vertexNum = (int)oVertices.size();
//			int idxNum = (int)oIndices.size();
//
//			// ��������
//			fwrite(&vertexNum, sizeof(int), 1, fileHandle);
//
//			// ��������
//			fwrite(&idxNum, sizeof(int), 1, fileHandle);
//
//			// ��������
//			fwrite(oVertices.data(), sizeof(OccVertexInput)*vertexNum, 1, fileHandle);
//
//			// ��������
//			fwrite(oIndices.data(), sizeof(Word)*idxNum, 1, fileHandle);
//
//			fflush(fileHandle);
//			fclose(fileHandle);
//
//			return true;
//		}
//
//		return false;
//	}
//
//#else
//	/**
//	 * ��Fbx�ļ��ж�ȡ�ڵ��޳�����
//	 */
//	bool importOccluderData(const char* fileName, vector<OccVertexInput>::type& oVertices, vector<Word>::type& oIndices)
//	{
//		DataStream* dataStream = ResourceGroupManager::instance()->openResource( fileName);
//		if( dataStream)
//		{
//			i32 vertexNum = 0;
//			i32 idxNum = 0;
//
//			dataStream->read( &vertexNum, sizeof(i32));
//			dataStream->read( &idxNum, sizeof(i32));
//
//			oVertices.resize(vertexNum);
//			oIndices.resize(idxNum);
//
//			// ����������
//			dataStream->read(oVertices.data(), sizeof(OccVertexInput)*vertexNum);
//
//			// ����������
//			dataStream->read(oIndices.data(), sizeof(Word)*idxNum);
//
//			EchoSafeDelete(dataStream, DataStream);
//		}
//
//		return false;
//	}
//
//#endif
//}}