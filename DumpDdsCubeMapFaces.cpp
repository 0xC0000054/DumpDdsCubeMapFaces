#include <iostream>
#include <filesystem>
#include <format>
#include "DirectXTex.h"
#include "wil/resource.h"

int wmain(int argc, wchar_t* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage DumpDdsCubeMapFaces <image.dds>" << std::endl;
		return 1;
	}

	auto autoCOM = wil::CoInitializeEx(COINIT_DISABLE_OLE1DDE);

	const std::filesystem::path ddsPath = argv[1];
	const std::filesystem::path imageRoot = ddsPath.parent_path();

	DirectX::TexMetadata texMetadata{};
	DirectX::ScratchImage image;

	if (SUCCEEDED(DirectX::LoadFromDDSFile(
		ddsPath.c_str(),
		DirectX::DDS_FLAGS_ALLOW_LARGE_FILES | DirectX::DDS_FLAGS_PERMISSIVE | DirectX::DDS_FLAGS_IGNORE_MIPS,
		&texMetadata,
		image)))
	{
		if (!texMetadata.IsCubemap())
		{
			std::wcout << ddsPath.native() << L"is not a cube map." << std::endl;
			return 1;
		}

		for (size_t i = 0; i < 6; i++)
		{
			const DirectX::Image* face = image.GetImage(0, i, 0);

			std::wstring fileName = std::format(L"face{0}.png", i);

			const std::filesystem::path output = imageRoot / fileName;

			DirectX::SaveToWICFile(
				*face,
				DirectX::WIC_FLAGS_NONE,
				DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG),
				output.c_str());
		}
	}

	return 0;
}