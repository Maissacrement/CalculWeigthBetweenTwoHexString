from build.calcWeight import getMatchingCase, getEntry

print(
    getMatchingCase('7f26374e1e5542b38fb82563411c6854b0'[:32], "./ids/guid.csv")
)

print(
    getEntry('7f26374e1e5542b38fb82563411c6854b0'[:6])
)