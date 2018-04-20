#Persistent
SetTimer, CloseMailWarnings, 2000
return

CloseMailWarnings:
print(111)
return

print(text)  
{  
    oSciTE:= ComObjActive("SciTE4AHK.Application")
    oSciTE.Output(text)
}