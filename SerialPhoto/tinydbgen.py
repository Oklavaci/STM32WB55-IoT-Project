
from ast import increment_lineno
from tinydb import TinyDB       
db = TinyDB("img_file.json")  # database

#   new_item = {"img_number": "Num", "value": 5} /to create new item in database
#   db.insert(new_item)     /to insert the item

#db.truncate()
#img_dict = {"name": "LastImage", "value": 0}
#db.insert(img_dict)    

from tinydb import Query
img = Query()

def getdbval():
    temp = db.search(img.name == "LastImage")
    return temp[0]['value']

def setdbval(val):
    if(val != 0):
        db.update({'value': val}, img.name == 'LastImage')


def updatedbval():
    db.update({'value':getdbval()+1 }, img.name == 'LastImage')

ToPrint = db.search(img.name == "LastImage")
print(ToPrint)

#setdbval(9)

ToPrint = db.search(img.name == "LastImage")
print(ToPrint)

