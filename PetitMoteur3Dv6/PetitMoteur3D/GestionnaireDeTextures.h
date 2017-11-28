#pragma once
#include <vector>
#include "texture.h"
#include "dispositifD3D11.h"

namespace PM3D
{

	class CGestionnaireDeTextures
	{
	private:
		CGestionnaireDeTextures() = default;
		~CGestionnaireDeTextures() = default;
	public:
		CGestionnaireDeTextures(const CGestionnaireDeTextures&) = delete;
		CGestionnaireDeTextures operator=(const CGestionnaireDeTextures&) = delete;

	public:
		static CGestionnaireDeTextures& GetInstance()
		{
			static CGestionnaireDeTextures gt;
			return gt;
		}

	public:
		CTexture* const GetNewTexture(const wchar_t* filename_in, CDispositifD3D11* pDispositif);
		CTexture* const GetTexture(const wchar_t* filename_in);
		void Cleanup();

	protected:
		// Le tableau de textures
		std::vector<CTexture*> ListeTextures;

	};

}


