/*++

Copyright (C) 2019 3MF Consortium

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
NMR_ModelReaderNode_Volumetric2201_Levelset.h covers the official 3MF volumetric extension.

--*/

#ifndef __NMR_MODELREADERNODE_Volumetric2201_LEVLSET
#define __NMR_MODELREADERNODE_Volumetric2201_LEVLSET

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelVolumeData.h"
#include "Common/Math/NMR_Matrix.h"

namespace NMR {

	class CModelReaderNode_Volumetric2201_Levelset : public CModelReaderNode {
	private:
		nfBool m_bHasStackId;
		nfBool m_bHasChannel;
		nfBool m_bHasSolidThreshold;
		nfBool m_bHasTransform;

		ModelResourceID m_nStackID;
		std::string m_sChannel;
		double m_dSolidThreshold;
		NMATRIX3 m_Transform;
	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
	public:
		CModelReaderNode_Volumetric2201_Levelset() = delete;
		CModelReaderNode_Volumetric2201_Levelset(_In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);

		PVolumeDataLevelset MakeLevelset(_In_ CModel* pModel);
	};

	typedef std::shared_ptr <CModelReaderNode_Volumetric2201_Levelset> PModelReaderNode_Volumetric2201_Levelset;

}

#endif // __NMR_MODELREADERNODE_Volumetric2201_LEVLSET

