from enum import Enum

class Nationality(Enum):
    brit = 0
    swede = 1
    dane = 2
    norwegian = 3
    german = 4

    def __int__(self):
        return self.value

class Color(Enum):
    red = 0
    green = 1
    yellow = 2
    blue = 3
    white = 4

    def __int__(self):
        return self.value

class Beverage(Enum):
    tea = 0
    coffee = 1
    milk = 2
    beer = 3
    water = 4

    def __int__(self):
        return self.value

class Cigar(Enum):
    pallmall = 0
    dunhill = 1
    blends = 2
    bluemasters = 3
    prince = 4

    def __int__(self):
        return self.value

class Pet(Enum):
    dogs = 0
    birds = 1
    cats = 2
    horse = 3
    fish = 4

    def __int__(self):
        return self.value

def make_einstein_prop(n=None, c=None, b=None, s=None, p=None, h=None):
    assert(isinstance(n, Nationality) or n is None)
    assert(isinstance(c, Color) or c is None)
    assert(isinstance(b, Beverage) or b is None)
    assert(isinstance(s, Cigar) or s is None)
    assert(isinstance(p, Pet) or p is None)
    assert(isinstance(h, int) or h is None)

    num_not_none = sum([x is not None for x in [n, c, b, s, p, h]])
    assert(num_not_none==2)

    prop_num = 0

    # Nationality
    if n is not None:
        if c is not None:
            # NC
            prop_num = prop_num + (int(n)*5) + int(c)
            return prop_num
        elif b is not None:
            # NB
            prop_num = 25
            prop_num = prop_num + (int(n)*5) + int(b)
            return prop_num
        elif s is not None:
            # NS
            prop_num = 50
            prop_num = prop_num + (int(n)*5) + int(s)
            return prop_num
        elif p is not None:
            # NP
            prop_num = 75
            prop_num = prop_num + (int(n)*5) + int(p)
            return prop_num
        else:
            # NH
            prop_num = 100
            prop_num = prop_num + (int(n)*5) + int(h)
            return prop_num

    # Color
    elif c is not None:
        prop_num = 125
        if b is not None:
            # CB
            prop_num = prop_num + (int(c)*5) + int(b)
            return prop_num
        elif s is not None:
            # CS
            prop_num = prop_num + 25
            prop_num = prop_num + (int(c)*5) + int(s)
            return prop_num
        elif p is not None:
            # CP
            prop_num = prop_num + 50
            prop_num = prop_num + (int(c)*5) + int(p)
            return prop_num
        else:
            # CH
            prop_num = prop_num + 75
            prop_num = prop_num + (int(c)*5) + int(h)
            return prop_num

    # Beverage
    elif b is not None:
        prop_num = 225
        if s is not None:
            # BS
            prop_num = prop_num + (int(b)*5) + int(s)
            return prop_num
        elif p is not None:
            # BP
            prop_num = prop_num + 25
            prop_num = prop_num + (int(b)*5) + int(p)
            return prop_num
        else:
            # BH
            prop_num = prop_num + 50
            prop_num = prop_num + (int(b)*5) + int(h)
            return prop_num

    # Smoking
    elif s is not None:
        prop_num = 300
        if p is not None:
            # SP
            prop_num = prop_num + (int(s)*5) + int(p)
            return prop_num
        else:
            # SH
            prop_num = prop_num + 25
            prop_num = prop_num + (int(s)*5) + int(h)
            return prop_num

    # Pet
    else:
        # PH
        prop_num = 350
        prop_num = prop_num + (int(p)*5) + int(h)


        return prop_num

def run():
    sol = [True, False, False, False, False, False, False, True, False, False, False, False, False, True, False, False, True, False, False, False, False, False, False, False, True, False, True, False, False, False, False, False, False, False, True, True, False, False, False, False, False, False, False, True, False, False, False, True, False, False, False, False, True, False, False, False, False, False, True, False, True, False, False, False, False, False, True, False, False, False, False, False, False, False, True, False, True, False, False, False, True, False, False, False, False, False, False, False, False, True, False, False, False, True, False, False, False, True, False, False, False, False, True, False, False, False, True, False, False, False, False, False, False, False, True, True, False, False, False, False, False, False, False, True, False, False, False, False, True, False, False, True, False, False, False, False, False, True, False, False, True, False, False, False, False, False, False, False, False, True, False, False, True, False, False, True, False, False, False, False, False, True, False, False, False, False, False, False, True, False, False, False, False, False, True, False, False, False, False, True, False, False, False, True, False, False, True, False, False, False, False, False, True, False, False, True, False, False, False, False, False, False, True, False, False, False, False, False, True, False, True, False, False, False, False, False, True, False, False, False, False, False, False, False, True, False, False, True, False, False, False, True, False, False, False, False, False, False, False, True, False, False, False, True, False, True, False, False, False, False, True, False, False, False, False, False, False, False, False, True, False, False, True, False, False, False, True, False, False, False, False, False, False, True, False, True, False, False, False, False, False, False, False, False, True, False, False, True, False, False, False, False, False, True, False, False, True, False, False, False, False, True, False, False, False, False, False, False, True, False, True, False, False, False, False, False, False, True, False, False, False, False, False, False, True, False, False, False, False, True, True, False, False, False, False, False, False, True, False, False, False, False, False, True, False, False, True, False, False, False, False, False, False, False, True, True, False, False, False, False, False, False, False, True, False, False, True, False, False, False, False, False, True, False, False]

    # TODO Interpret solution

if __name__ == "__main__":
    run()
