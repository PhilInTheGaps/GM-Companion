'''
Created on 08.04.2017

@author: Phil
'''

import os
from root.nested import another_test

if __name__ == "__main__":
    print("Hello World")
    
s = "HALLO WELT; DAS HIER IST EIN STRING"
print(s)

int1 = 5
int2 = 4
intsum = int1+int2
print(intsum)
print()

# A Comment
for i in range(0, 9):
    print(i)

def test_function():
    """This function does not do anything useful yet"""
    print("TEST!!!")
    
print(test_function.__doc__)
test_function()

# Prints the type of int1
print(type(int1))

s = "TEST STRING"
print(s)

num = input("Enter a number")
print(num)

another_test.func_test()

print()
print("Current Working Directory: "+os.getcwd())

