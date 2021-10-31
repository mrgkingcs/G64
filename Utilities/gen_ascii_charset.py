myFile = open("system-charset.bin", "rb")
unshiftedFileBytes = myFile.read()
myFile.close()

myFile = open("system-charset-lower.bin", "rb")
shiftedFileBytes = myFile.read()
myFile.close()

asciiBytes = [0]*8*32    # control characters
asciiBytes += shiftedFileBytes[32*8:64*8]   # punctuation and numbers
asciiBytes += shiftedFileBytes[64*8:91*8]   # upper case
asciiBytes += unshiftedFileBytes[27*8:28*8] # [
asciiBytes += [0]*8   # \
asciiBytes += unshiftedFileBytes[29*8:30*8] # ]
asciiBytes += [0]*8   # ^
asciiBytes += [0]*8   # _
asciiBytes += shiftedFileBytes[0*8:27*8] # lower case
asciiBytes += [0]*8   # {
asciiBytes += [0]*8   # |
asciiBytes += [0]*8   # }
asciiBytes += [0]*8   # ~
asciiBytes += [0]*8   # DEL
asciiBytes += [0]*8*128




print("byte charset_ascii[256*8] = {")

for startByte in range(0, 256*8, 8):
    lineStr = "\t\t"
    charBytes = asciiBytes[startByte:startByte+8]
    for byte in charBytes:
        lineStr += "0x{:02x}, ".format(byte)
    print(lineStr)

print("\t};")

