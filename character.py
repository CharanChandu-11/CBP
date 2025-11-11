#Character framing
s=input("").split()
for i in s:
    print(f"{len(i)}{i}",end=" ")


#Character stuffing
s=input("").split()
for i in s:
    if i in ('esc','flag'):
        print(i,i,end=" ")
    else:
        print(i,end=" ")

#Bit stuffing
pat=input()
n=len(pat)
req=pat[:n-1]
ad=pat[n-1]
a=""
s=input()
for i in s:
    if pat in a:
        print(f"{a}{ad}",end="")
        a=i
    else:
        a+=i
print(a)
