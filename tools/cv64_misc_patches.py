import sys
import os

ROM = sys.argv[1]
creator = sys.argv[2]
hook = sys.argv[3]

def writeBytesToROM(value, address, size, ROM_file):
	ROM_file.seek(address)
	ROM_file.write(value)

def patch_patchEntrypointAddress(ROM_file):
	writeBytesToROM(0x0F000000.to_bytes(4, 'big'), 0x93374, 4, ROM_file)

def patch_patchGameStateSettings(ROM_file):
	writeBytesToROM(0x40000004.to_bytes(4, 'big'), 0x92C3C, 4, ROM_file)
	writeBytesToROM(0x0700001F.to_bytes(4, 'big'), 0x92C5C, 4, ROM_file)

def patch_createFileInfo(ROM_file):
	writeBytesToROM(0x8018C0A0.to_bytes(4, 'big'), 0x94530, 4, ROM_file)
	writeBytesToROM(0x40000071.to_bytes(4, 'big'), 0x10F290, 4, ROM_file)
	writeBytesToROM(0x00001000.to_bytes(4, 'big'), 0x10F294, 4, ROM_file)

def patch_patchCreator(creator_file_buffer, ROM_file):
	writeBytesToROM(creator_file_buffer.read(), 0x10F190, os.path.getsize(creator), ROM_file)

def patch_patchCreatorEntrypoint(ROM_file):
	writeBytesToROM(0x8018BFA0.to_bytes(4, 'big'), 0x92EEC, 4, ROM_file)

def patch_patchAddHook(ROM_file):
	writeBytesToROM(0xA018BFE0.to_bytes(4, 'big'), 0x92BD8, 4, ROM_file)

def patch_patchAddHookCode(hook_code_file_buffer, ROM_file):
	writeBytesToROM(hook_code_file_buffer.read(), 0x10F1D0, os.path.getsize(hook), ROM_file)

if __name__ == "__main__":
	with open(ROM, "r+b") as ROM_file:
		patch_patchEntrypointAddress(ROM_file)
		patch_patchGameStateSettings(ROM_file)
		patch_createFileInfo(ROM_file)

		with open(creator, "r+b") as creator_file_buffer:
			patch_patchCreatorEntrypoint(ROM_file)
			patch_patchCreator(creator_file_buffer, ROM_file)
		
		with open(hook, "r+b") as hook_code_file_buffer:
			patch_patchAddHook(ROM_file)
			patch_patchAddHookCode(hook_code_file_buffer, ROM_file)