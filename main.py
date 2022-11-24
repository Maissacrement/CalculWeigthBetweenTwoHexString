from build.calcWeight import getMatchingCase, getEntry

"""
print(
    getMatchingCase('7f26374e1e5542b38fb82563411c6854b0'[:32], "./ids/guid.csv")
)
"""
ws='14e914e9004531ea003000000002000000000000095'
res=0
err=-1

print(len(ws))

for i in range(len(ws)):
    if(i + 6 > len(ws)): break
    mpkg=ws[i:i+6]
    r=getEntry(mpkg).split(',')
    xored=r[2].split(';')[:-1]
    
    for j in range(len(xored)//2):
        m=int(mpkg[j*2:(j*2)+2], 16)
        if(err>int(r[0]) or err==-1):
            #res=( m^int(xored[j*2]) ) - int(xored[(j*2)+1])
            res=r+[mpkg, i]
            err=int(r[0])

nch=""
w=ws[:res[-1]][::-1]    
for i in range(res[-1] // 2):
    xored=res[2].split(';')[:-1]
    wsc=w[i*2:i*2+2]
    for j in range(3):
        p=int(xored[j*2])^int(int(wsc, 16))
        if (p >= int(res[1][j*2:j*2+2], 16)): 
            r=chr(p - int(xored[(j*2)+1]))
        else:
            r=chr(p + int(xored[(j*2)+1]))
        nch=str(r)+nch

w=ws[res[-1]:res[-1]+6]
for i in range(3):
    xored=res[2].split(';')[:-1]
    wsc=w[i*2:i*2+2]
    p=int(xored[i*2])^int(int(wsc, 16))
    if (p >= int(res[1][i*2:i*2+2], 16)): 
        r=chr(p - int(xored[(i*2)+1]))
    else:
        r=chr(p + int(xored[(i*2)+1]))
    nch+=str(r)

w=ws[res[-1]:len(ws)]
for i in range((len(ws) - res[-1]) // 2):
    xored=res[2].split(';')[:-1]
    wsc=w[i*2:i*2+2]
    for j in range(3):
        p=int(xored[j*2])^int(int(wsc, 16))
        if (p >= int(res[1][j*2:j*2+2], 16)): 
            r=chr(p - int(xored[(j*2)+1]))
        else:
            r=chr(p + int(xored[(j*2)+1]))
        nch+=str(r)

print(nch)
