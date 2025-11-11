# Input
data = [int(x) for x in input("Enter data bits: ")]
divisor = [int(x) for x in input("Enter divisor bits: ")]

# Prepare
r = len(divisor)
a = data + [0]*(r-1)

# CRC Division
for i in range(len(data)):
    if a[i] == 1:
        for j in range(r):
            a[i+j] ^= divisor[j]

# Result
remainder = a[-(r-1):]
codeword = data + remainder

print("Remainder:", remainder)
print("Codeword:", codeword)
