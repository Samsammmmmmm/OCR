def fixText(text):
    row = []
    z = text.find(',')
    if z == 0:  row.append('')
    else:   row.append(text[:z])
    for x in range(len(text)):
        if text[x] != ',':  pass
        else:
            if x == (len(text)-1):  row.append('')
            else:
                if ',' in text[(x+1):]:
                    y = text.find(',', (x+1))
                    c = text[(x+1):y]
                else:   c = text[(x+1):]
                row.append(c)
    return row

def createTuple(oldFile):
    ## oldFile is filename (e.g. 'sheet.csv')
    f1 = open(oldFile, "r")
    tup = []
    while 1:
        text = f1.readline()
        if text == "":  break
        else:   pass
        if text[-1] == '\n':
            text = text[:-1]
        else:   pass
        row = fixText(text)
        tup.append(row)
    return tup


def write_csv():
    t = createTuple("TMNIST.csv")
    rslt = "{"
    stri = "{"
    for i in range(1, 5):
        stri += '{'
        rslt += t[i][0]
        for j in range(1,len(t[0])):
            if t[i][j] != '0':
                stri += '1.0'
            else:
                stri += '0.0'
            if j != len(t[0]) - 1:
                stri += ','
        stri+= "}"
        if i != 4:
            rslt+=','
            stri+= ','
    stri+= "}"
    rslt+='}'
    print(rslt)
    print(stri)

write_csv()


