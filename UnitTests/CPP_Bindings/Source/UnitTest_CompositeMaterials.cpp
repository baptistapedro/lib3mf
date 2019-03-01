/*++

Copyright (C) 2018 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

UnitTest_CompositeMaterials.cpp: Defines Unittests for Composite Materials

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class CompositeMaterials : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();

			std::vector<sLib3MFPosition> vctVertices;
			std::vector<sLib3MFTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());


			baseMaterialGroup1 = model->AddBaseMaterialGroup();
			baseMaterialGroup2 = model->AddBaseMaterialGroup();

			sLib3MFColor tIn;
			tIn.m_Red = 20;
			tIn.m_Blue = 10;
			tIn.m_Green = 100;
			tIn.m_Alpha = 255;
			baseMaterialGroup1->AddMaterial("Material1", tIn);
			tIn.m_Green = 200;
			baseMaterialGroup1->AddMaterial("Material2", tIn);

			tIn.m_Red = 250;
			tIn.m_Blue = 10;
			tIn.m_Green = 0;
			tIn.m_Alpha = 255;
			baseMaterialGroup2->AddMaterial("MaterialB1", tIn);
			tIn.m_Blue = 200;
			baseMaterialGroup2->AddMaterial("MaterialB2", tIn);
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFMeshObject mesh;
		PLib3MFBaseMaterialGroup baseMaterialGroup1, baseMaterialGroup2;
	};

	TEST_F(CompositeMaterials, Create)
	{
		auto compositeMaterial = model->AddCompositeMaterials(baseMaterialGroup1.get());

		auto baseMaterialGroup1_Out = compositeMaterial->GetBaseMaterialGroup();
		ASSERT_EQ(baseMaterialGroup1_Out->GetResourceID(), baseMaterialGroup1->GetResourceID());
	}

	void compareConstituents(std::vector<sLib3MFCompositeConstituent> constituentsA,  std::vector<sLib3MFCompositeConstituent> constituentsB)
	{
		ASSERT_EQ(constituentsA.size(), constituentsB.size());
		for (int i = 0; i < constituentsA.size(); i++) {
			ASSERT_EQ(constituentsA[i].m_PropertyID, constituentsB[i].m_PropertyID);
			ASSERT_DOUBLE_EQ(constituentsA[i].m_MixingRatio, constituentsB[i].m_MixingRatio);
		}
	}

	TEST_F(CompositeMaterials, AddGet_Composites)
	{
		auto compositeMaterial = model->AddCompositeMaterials(baseMaterialGroup1.get());
		ASSERT_EQ(compositeMaterial->GetCount(), 0);


		std::vector<sLib3MFCompositeConstituent> constituents(1);
		constituents[0].m_MixingRatio = 0.5;
		constituents[0].m_PropertyID = 100;

		ASSERT_SPECIFIC_THROW(compositeMaterial->AddComposite(constituents), ELib3MFException);
		constituents[0].m_PropertyID = 1;
		Lib3MF_uint32 propertyID1 = compositeMaterial->AddComposite(constituents);

		constituents.push_back(sLib3MFCompositeConstituent({2, 0.2 }));
		Lib3MF_uint32 propertyID2 = compositeMaterial->AddComposite(constituents);

		ASSERT_EQ(compositeMaterial->GetCount(), 2);

		std::vector<sLib3MFCompositeConstituent> outConstituents;
		compositeMaterial->GetComposite(propertyID2, outConstituents);

		std::vector<Lib3MF_uint32> outPropertyIDs;
		compositeMaterial->GetAllPropertyIDs(outPropertyIDs);
		ASSERT_EQ(2, outPropertyIDs.size());
		ASSERT_EQ(propertyID1, outPropertyIDs[0]);
		ASSERT_EQ(propertyID2, outPropertyIDs[1]);
	}

	

	//TEST_F(CompositeMaterials, SetGet_CompositeMaterialGroup)
	//{
	//	std::vector<sLib3MFTex2Coord> coords(0);

	//	auto texture2DGroup = model->AddTexture2DGroup(texture2D.get());
	//	std::vector<sLib3MFTriangleProperties> properties(mesh->GetTriangleCount());
	//	for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
	//		properties[i].m_ResourceID = texture2DGroup->GetResourceID();

	//		coords.push_back({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
	//		properties[i].m_PropertyIDs[0] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
	//		coords.push_back({ 1.0*(i + 1) / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
	//		properties[i].m_PropertyIDs[1] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
	//		coords.push_back({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*(i + 1) / mesh->GetTriangleCount() });
	//		properties[i].m_PropertyIDs[2] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
	//	}
	//	mesh->SetAllTriangleProperties(properties);

	//	std::vector<sLib3MFTriangleProperties> gottenProperties;
	//	mesh->GetAllTriangleProperties(gottenProperties);
	//	int count = 0;
	//	for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
	//		EXPECT_EQ(gottenProperties[i].m_ResourceID, properties[i].m_ResourceID);
	//		for (Lib3MF_uint64 j = 0; j < 3; j++) {
	//			EXPECT_EQ(gottenProperties[i].m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
	//		}

	//		sLib3MFTriangleProperties currentProperty;
	//		mesh->GetTriangleProperties(Lib3MF_uint32(i), currentProperty);
	//		auto currentTexture2DGroup = model->GetTexture2DGroupByID(currentProperty.m_ResourceID);
	//		EXPECT_EQ(currentProperty.m_ResourceID, properties[i].m_ResourceID);
	//		for (Lib3MF_uint64 j = 0; j < 3; j++) {
	//			EXPECT_EQ(currentProperty.m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
	//			sLib3MFTex2Coord uvcoord = currentTexture2DGroup->GetTex2Coord(currentProperty.m_PropertyIDs[j]);
	//			EXPECT_DOUBLE_EQ(uvcoord.m_U, coords[count].m_U);
	//			EXPECT_DOUBLE_EQ(uvcoord.m_V, coords[count++].m_V);
	//		}
	//	}

	//	std::vector<Lib3MF_uint32> properties2D;
	//	texture2DGroup->GetAllPropertyIDs(properties2D);
	//}

	//TEST_F(CompositeMaterials, WriteRead)
	//{

	//	ASSERT_SPECIFIC_THROW(texture->SetAttachment(otherAttachment.get()), ELib3MFException);

	//	auto texture2DGroup = model->AddTexture2DGroup(texture2D.get());
	//	std::vector<sLib3MFTriangleProperties> properties(mesh->GetTriangleCount());
	//	for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
	//		properties[i].m_ResourceID = texture2DGroup->GetResourceID();
	//		properties[i].m_PropertyIDs[0] = texture2DGroup->AddTex2Coord({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
	//		properties[i].m_PropertyIDs[1] = texture2DGroup->AddTex2Coord({ 1.0*(i + 1) / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
	//		properties[i].m_PropertyIDs[2] = texture2DGroup->AddTex2Coord({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*(i + 1) / mesh->GetTriangleCount() });
	//	}
	//	mesh->SetAllTriangleProperties(properties);

	//	auto writer = model->QueryWriter("3mf");
	//	std::vector<Lib3MF_uint8> buffer;
	//	writer->WriteToBuffer(buffer);
	//	// writer->WriteToFile("Texture_Out.3mf");

	//	auto readModel = CLib3MFWrapper::CreateModel();
	//	auto reader = readModel->QueryReader("3mf");
	//	// reader->ReadFromFile("Texture_Out.3mf");
	//	reader->ReadFromBuffer(buffer);

	//	int texture2DGroupCount = 0;
	//	auto iterator = readModel->GetTexture2DGroups();
	//	while (iterator->MoveNext())
	//	{
	//		auto texture2Dgroup = iterator->GetCurrentTexture2DGroup();
	//		ASSERT_EQ(texture2Dgroup->GetCount(), properties.size()*3);
	//		texture2DGroupCount++;
	//		auto localTexture2D = texture2Dgroup->GetTexture2D();
	//		ASSERT_EQ(localTexture2D->GetAttachment()->GetStreamSize(), texture2D->GetAttachment()->GetStreamSize());
	//	}
	//	ASSERT_EQ(texture2DGroupCount, 1);
	//}
	
}
