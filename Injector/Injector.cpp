#include <Windows.h>

INT main(INT argc, PCHAR argv[])
{
	//Injector.exe "section name" "target process" "dll" 

	if (argv[0] && argv[1] && argv[2] && argv[3])
	{
		char* target_process = argv[2];
		char* dll_path = argv[3];
		PCHAR section_name = argv[1];
		//create file handle

		HANDLE x_file = CreateFileA(target_process, GENERIC_READ | GENERIC_WRITE, 0, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (x_file != INVALID_HANDLE_VALUE)
		{
			DWORD file_size = GetFileSize(x_file, NULL);
			PBYTE file_buffer = PBYTE(LocalAlloc(LPTR, file_size));
			DWORD return_bytes;

			BOOL file_read = ReadFile(x_file, file_buffer, file_size, &return_bytes, NULL);
			if (file_read == TRUE && return_bytes == file_size)
			{
				PIMAGE_DOS_HEADER image_dos_header = (PIMAGE_DOS_HEADER) file_read;
				if (image_dos_header->e_magic == IMAGE_DOS_SIGNATURE)
				{
					//if PE is valid
					PIMAGE_FILE_HEADER image_file_header = (PIMAGE_FILE_HEADER)
						(file_read + image_dos_header->e_lfanew + sizeof(DWORD));
					PIMAGE_OPTIONAL_HEADER image_optional_header = (PIMAGE_OPTIONAL_HEADER)
						(file_read + image_dos_header->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
					PIMAGE_SECTION_HEADER image_section_header = (PIMAGE_SECTION_HEADER)
						(file_read + image_dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS));
					WORD PESections = image_file_header->NumberOfSections;

					ZeroMemory(&image_section_header[PESections], sizeof(IMAGE_SECTION_HEADER));
					CopyMemory(&image_section_header[PESections].Name, section_name, 8);

					HANDLE code_file = CreateFileA(dll_path, GENERIC_READ, 0, NULL, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, NULL);
					if (code_file != INVALID_HANDLE_VALUE)
					{
						DWORD dll_size = GetFileSize(code_file, 0);
						if (dll_size > 0)
						{
							PBYTE dll_buffer = PBYTE(LocalAlloc(LPTR, dll_size));
							if (dll_buffer != ERROR)
							{
								DWORD returned_dll_bytes;
								BOOL dll_read = ReadFile(code_file, dll_buffer, dll_size
									, &returned_dll_bytes, NULL);
								if (dll_read == TRUE && returned_dll_bytes == dll_size)
								{
									//first set file pointer inside section header
									if (SetFilePointer(x_file, image_section_header[PESections].PointerToRawData + image_section_header[PESections].SizeOfRawData, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER)
									{

									}
								}
							}
						}

					}
				}
			}
		}
	}
}