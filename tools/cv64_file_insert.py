import sys
import subprocess
import os

# Constants
ROM_size = 0xC00000
number_of_NI_files = 254

# Offsets
## USA v1.0
NisitenmaIchigo_table_start_addr = 0x95C34
file_size_table_start_addr = 0x95430

def compressFile(filename):
	subprocess.run(["./tools/lzkn64", "-c", filename, "temp"])
	new_file = open("temp", "rb")
	fileBuffer = new_file.read()
	new_file.close()
	return fileBuffer

# Write a file buffer to the ROM
def writeFile(ROM_file, input_file, addr):
	ROM_file.seek(addr)
	ROM_file.write(input_file)

# Patch the Nisitenma-Ichigo table entry corresponding to the file ID
# with the start and end addresses of the new injected file
# 1 entry in the table = size 0x8
def patchNITableAddresses(ROM_file, file_start_addr, file_end_addr):
	table_entry_addr = NisitenmaIchigo_table_start_addr + (8 * file_ID)
	ROM_file.seek(table_entry_addr)
	ROM_file.write((0x80000000 | file_start_addr).to_bytes(4, 'big'))	# 0x80000000 --> File is compressed
	ROM_file.seek(table_entry_addr + 4)
	ROM_file.write(file_end_addr.to_bytes(4, 'big'))

# Patch table containing the file sizes with the file size of the new injected file (decompressed)
# 1 entry in the table = size 0x8
def patchFileSizeTable(ROM_file, file_size):
	table_entry_addr = file_size_table_start_addr + (8 * file_ID)
	ROM_file.seek(table_entry_addr)
	segment_ID = int.from_bytes(ROM_file.read(4), 'big') & 0xFF000000	# Get segment ID of the file
	ROM_file.write((segment_ID | file_size).to_bytes(4, 'big'))			# Write segment ID | file size

def checkArgsErrors():
	if injection_offset > ROM_size:
			raise Exception('Invalid injection offset')
	if file_ID > number_of_NI_files or file_ID <= 0:
			raise Exception('Invalid file ID')

def main():
	with open(ROM, "r+b") as rom_file:
		# Get file size of the file (decompressed),
		# which we need to patch in the file size table later
		decompressed_size = os.path.getsize(file)
		fileBuffer = compressFile(file)

		file_end_addr = injection_offset + len(fileBuffer)

		if file_end_addr > ROM_size:
			raise Exception('The file to inject is too big, or it doesnt fit entirely if injected at:', hex(injection_offset))

		writeFile(rom_file, fileBuffer, injection_offset)
		patchNITableAddresses(rom_file, injection_offset, file_end_addr)
		patchFileSizeTable(rom_file, decompressed_size)
		# Remove temporary file
		subprocess.run(["rm", "-f", "temp"])

if __name__ == "__main__":
	if len(sys.argv) != 5:
		print("CV64 File Injector")
		print("")
		print("python3 cv64_file_insert.py ROM_file file_to_inject injection_address file_ID")
		print("NOTE: All numbers are in hexadecimal, without the \"0x\"")
		print("		ROM_file: Path to the CV64 USA v1.0 ROM file")
		print("		file_to_inject: Path to the file to be injected into the ROM")
		print("		injection_address: Address in the ROM to inject \"file_to_inject\"")
		print("		file_ID: ID of the file to replace")

		print("\nExample: python3 cv64_file_insert.py cv64.z64 my_new_gardener.bin BC9800 C")
		print("Injects \"my_new_gardener.bin\" into address 0xBC9800 in the ROM, and replaced file 0xC")
	else:
		# Arguments
		ROM = sys.argv[1]
		file = sys.argv[2]
		injection_offset = int(sys.argv[3], 16)
		file_ID = int(sys.argv[4], 16)

		checkArgsErrors()
		main()
	