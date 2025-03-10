import sys
sys.path.append('./build')

import hello

def test_hello():
    hello.say_hello()

if __name__ == '__main__':
    test_hello()