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

class Clause:
    def __init__(self):
        self.s = set([])

    def __str__(self):
        ret_str = ""
        for p in self.s:
            ret_str += str(p)
            ret_str += " "
        ret_str += "0"
        return ret_str
    
    def __repr__(self):
        ret_str = ""
        for p in self.s:
            ret_str += str(p)
            ret_str += " "
        ret_str += "0"
        return ret_str

    def add_literal(self, l):
        self.s.add(l)

def make_literal(prop, negated=False):
    if negated:
        return -(prop + 1)
    else:
        return prop + 1
        
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

def make_exclusion_axioms(ns=None, cs=None, bs=None, ss=None, ps=None, hs=None):
    num_not_none = sum([x is not None for x in [ns, cs, bs, ss, ps, hs]])
    assert(num_not_none==2)

    axioms = []

    # Nationality
    if ns is not None:
        if cs is not None:
            # NC
            for c in cs:
                disjunctive_c = Clause()
                for i in range(len(ns)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], c=c)))
                    for j in range(i+1, len(ns)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], c=c), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[j], c=c), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for n in ns:
                disjunctive_c = Clause()
                for i in range(len(cs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=n, c=cs[i])))
                    for j in range(i+1, len(cs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, c=cs[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, c=cs[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)

        elif bs is not None:
            # NB
            for b in bs:
                disjunctive_c = Clause()
                for i in range(len(ns)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], b=b)))
                    for j in range(i+1, len(ns)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], b=b), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[j], b=b), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for n in ns:
                disjunctive_c = Clause()
                for i in range(len(bs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=n, b=bs[i])))
                    for j in range(i+1, len(bs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, b=bs[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, b=bs[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        elif ss is not None:
            # NS
            for s in ss:
                disjunctive_c = Clause()
                for i in range(len(ns)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], s=s)))
                    for j in range(i+1, len(ns)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], s=s), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[j], s=s), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for n in ns:
                disjunctive_c = Clause()
                for i in range(len(ss)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=n, s=ss[i])))
                    for j in range(i+1, len(ss)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, s=ss[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, s=ss[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        elif ps is not None:
            # NP
            for p in ps:
                disjunctive_c = Clause()
                for i in range(len(ns)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], p=p)))
                    for j in range(i+1, len(ns)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], p=p), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[j], p=p), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for n in ns:
                disjunctive_c = Clause()
                for i in range(len(ps)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=n, p=ps[i])))
                    for j in range(i+1, len(ps)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, p=ps[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, p=ps[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        else:
            # NH
            for h in hs:
                disjunctive_c = Clause()
                for i in range(len(ns)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], h=h)))
                    for j in range(i+1, len(ns)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[i], h=h), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=ns[j], h=h), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for n in ns:
                disjunctive_c = Clause()
                for i in range(len(hs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(n=n, h=hs[i])))
                    for j in range(i+1, len(hs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, h=hs[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(n=n, h=hs[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)

    # Color
    elif cs is not None:
        if bs is not None:
            # CB
            for b in bs:
                disjunctive_c = Clause()
                for i in range(len(cs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], b=b)))
                    for j in range(i+1, len(cs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], b=b), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[j], b=b), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for c in cs:
                disjunctive_c = Clause()
                for i in range(len(bs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=c, b=bs[i])))
                    for j in range(i+1, len(bs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, b=bs[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, b=bs[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        elif ss is not None:
            # CS
            for s in ss:
                disjunctive_c = Clause()
                for i in range(len(cs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], s=s)))
                    for j in range(i+1, len(cs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], s=s), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[j], s=s), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for c in cs:
                disjunctive_c = Clause()
                for i in range(len(ss)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=c, s=ss[i])))
                    for j in range(i+1, len(ss)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, s=ss[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, s=ss[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        elif ps is not None:
            # CP
            for p in ps:
                disjunctive_c = Clause()
                for i in range(len(cs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], p=p)))
                    for j in range(i+1, len(cs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], p=p), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[j], p=p), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for c in cs:
                disjunctive_c = Clause()
                for i in range(len(ps)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=c, p=ps[i])))
                    for j in range(i+1, len(ps)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, p=ps[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, p=ps[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        else:
            # CH
            for h in hs:
                disjunctive_c = Clause()
                for i in range(len(cs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], h=h)))
                    for j in range(i+1, len(cs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[i], h=h), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=cs[j], h=h), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for c in cs:
                disjunctive_c = Clause()
                for i in range(len(hs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(c=c, h=hs[i])))
                    for j in range(i+1, len(hs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, h=hs[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(c=c, h=hs[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)

    # Beverage
    elif bs is not None:
        if ss is not None:
            # BS
            for s in ss:
                disjunctive_c = Clause()
                for i in range(len(bs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(b=bs[i], s=s)))
                    for j in range(i+1, len(bs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=bs[i], s=s), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=bs[j], s=s), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for b in bs:
                disjunctive_c = Clause()
                for i in range(len(ss)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(b=b, s=ss[i])))
                    for j in range(i+1, len(ss)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=b, s=ss[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=b, s=ss[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        elif ps is not None:
            # BP
            for p in ps:
                disjunctive_c = Clause()
                for i in range(len(bs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(b=bs[i], p=p)))
                    for j in range(i+1, len(bs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=bs[i], p=p), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=bs[j], p=p), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for b in bs:
                disjunctive_c = Clause()
                for i in range(len(ps)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(b=b, p=ps[i])))
                    for j in range(i+1, len(ps)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=b, p=ps[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=b, p=ps[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        else:
            # BH
            for h in hs:
                disjunctive_c = Clause()
                for i in range(len(bs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(b=bs[i], h=h)))
                    for j in range(i+1, len(bs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=bs[i], h=h), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=bs[j], h=h), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for b in bs:
                disjunctive_c = Clause()
                for i in range(len(hs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(b=b, h=hs[i])))
                    for j in range(i+1, len(hs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=b, h=hs[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(b=b, h=hs[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)

    # Smoking
    elif ss is not None:
        if ps is not None:
            # SP
            for p in ps:
                disjunctive_c = Clause()
                for i in range(len(ss)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(s=ss[i], p=p)))
                    for j in range(i+1, len(ss)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=ss[i], p=p), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=ss[j], p=p), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for s in ss:
                disjunctive_c = Clause()
                for i in range(len(ps)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(s=s, p=ps[i])))
                    for j in range(i+1, len(ps)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=s, p=ps[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=s, p=ps[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
        else:
            # SH
            for h in hs:
                disjunctive_c = Clause()
                for i in range(len(ss)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(s=ss[i], h=h)))
                    for j in range(i+1, len(ss)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=ss[i], h=h), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=ss[j], h=h), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)
            for s in ss:
                disjunctive_c = Clause()
                for i in range(len(hs)):
                    disjunctive_c.add_literal(make_literal(make_einstein_prop(s=s, h=hs[i])))
                    for j in range(i+1, len(hs)):
                        exclusive_c = Clause()
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=s, h=hs[i]), negated=True))
                        exclusive_c.add_literal(make_literal(make_einstein_prop(s=s, h=hs[j]), negated=True))
                        axioms.append(exclusive_c)
                axioms.append(disjunctive_c)

    # Pet
    else:
        # PH
        for h in hs:
            disjunctive_c = Clause()
            for i in range(len(ps)):
                disjunctive_c.add_literal(make_literal(make_einstein_prop(p=ps[i], h=h)))
                for j in range(i+1, len(ps)):
                    exclusive_c = Clause()
                    exclusive_c.add_literal(make_literal(make_einstein_prop(p=ps[i], h=h), negated=True))
                    exclusive_c.add_literal(make_literal(make_einstein_prop(p=ps[j], h=h), negated=True))
                    axioms.append(exclusive_c)
            axioms.append(disjunctive_c)
        for p in ps:
            disjunctive_c = Clause()
            for i in range(len(hs)):
                disjunctive_c.add_literal(make_literal(make_einstein_prop(p=p, h=hs[i])))
                for j in range(i+1, len(hs)):
                    exclusive_c = Clause()
                    exclusive_c.add_literal(make_literal(make_einstein_prop(p=p, h=hs[i]), negated=True))
                    exclusive_c.add_literal(make_literal(make_einstein_prop(p=p, h=hs[j]), negated=True))
                    axioms.append(exclusive_c)
            axioms.append(disjunctive_c)

    return axioms

# TODO Implement consistency axioms, which should say that (p_{n,c} and p_{n, p}) \implies p_{c, p}) 
# I.e. (\neg p_{n, c} \vee \neg p_{n, p} \vee p_{c, p}) for all combinations of pairs

def make_consistency_axioms():
    axioms = []
    for i in range(5):
        for j in range(5):
            for k in range(5):
                # NC
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(c=Color(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), c=Color(k))))
                axioms.append(c)

                # NB
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(b=Beverage(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), b=Beverage(k))))
                axioms.append(c)
                
                # NS
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(s=Cigar(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), s=Cigar(k))))
                axioms.append(c)

                # NP
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(p=Pet(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(n=Nationality(j), p=Pet(k))))
                axioms.append(c)

                # CB
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(c=Color(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(b=Beverage(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(c=Color(j), b=Beverage(k))))
                axioms.append(c)

                # CS
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(c=Color(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(s=Cigar(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(c=Color(j), s=Cigar(k))))
                axioms.append(c)

                # CP
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(c=Color(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(p=Pet(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(c=Color(j), p=Pet(k))))
                axioms.append(c)

                # BS
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(b=Beverage(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(s=Cigar(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(b=Beverage(j), s=Cigar(k))))
                axioms.append(c)

                # BP
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(b=Beverage(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(p=Pet(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(b=Beverage(j), p=Pet(k))))
                axioms.append(c)

                # SP
                c = Clause()
                c.add_literal(make_literal(make_einstein_prop(s=Cigar(j), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(p=Pet(k), h=i), negated=True))
                c.add_literal(make_literal(make_einstein_prop(s=Cigar(j), p=Pet(k))))
                axioms.append(c)

    return axioms



def make_einstein_axioms():
    axioms = []

    ## Basic exclusion axioms
    ns = [Nationality.brit, Nationality.swede, Nationality.dane, Nationality.norwegian,  Nationality.german]
    cs = [Color.red, Color.green, Color.yellow, Color.blue, Color.white]
    bs = [Beverage.tea, Beverage.coffee, Beverage.milk, Beverage.beer, Beverage.water]
    ss = [Cigar.pallmall, Cigar.dunhill, Cigar.blends, Cigar.bluemasters, Cigar.prince]
    ps = [Pet.dogs, Pet.birds, Pet.cats, Pet.horse, Pet.fish]
    hs = [0, 1, 2, 3, 4]

    axioms = axioms + make_exclusion_axioms(ns=ns, cs=cs)
    axioms = axioms + make_exclusion_axioms(ns=ns, bs=bs)
    axioms = axioms + make_exclusion_axioms(ns=ns, ss=ss)
    axioms = axioms + make_exclusion_axioms(ns=ns, ps=ps)
    axioms = axioms + make_exclusion_axioms(ns=ns, hs=hs)

    axioms = axioms + make_exclusion_axioms(cs=cs, bs=bs)
    axioms = axioms + make_exclusion_axioms(cs=cs, ss=ss)
    axioms = axioms + make_exclusion_axioms(cs=cs, ps=ps)
    axioms = axioms + make_exclusion_axioms(cs=cs, hs=hs)

    axioms = axioms + make_exclusion_axioms(bs=bs, ss=ss)
    axioms = axioms + make_exclusion_axioms(bs=bs, ps=ps)
    axioms = axioms + make_exclusion_axioms(bs=bs, hs=hs)

    axioms = axioms + make_exclusion_axioms(ss=ss, ps=ps)
    axioms = axioms + make_exclusion_axioms(ss=ss, hs=hs)

    axioms = axioms + make_exclusion_axioms(ps=ps, hs=hs)
    
    # Consistency axioms
    axioms = axioms + make_consistency_axioms()

    ## Fifteen facts
    # Brit lives in red house
    c1 = Clause()
    c1.add_literal(make_literal(make_einstein_prop(n=Nationality.brit, c=Color.red)))
    axioms.append(c1)

    # Swede keeps dogs
    c2 = Clause()
    c2.add_literal(make_literal(make_einstein_prop(n=Nationality.swede, p=Pet.dogs)))
    axioms.append(c2)

    # Dane drinks tea
    c3 = Clause()
    c3.add_literal(make_literal(make_einstein_prop(n=Nationality.dane, b=Beverage.tea)))
    axioms.append(c3)

    # Green house on the left of white house
    c4 = Clause()
    c4.add_literal(make_literal(make_einstein_prop(c=Color.white, h=1), negated=True))
    axioms.append(c4)

    c4 = Clause()
    c4.add_literal(make_literal(make_einstein_prop(c=Color.white, h=2), negated=True))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=1)))
    axioms.append(c4)

    c4 = Clause()
    c4.add_literal(make_literal(make_einstein_prop(c=Color.white, h=3), negated=True))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=2)))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=1)))
    axioms.append(c4)

    c4 = Clause()
    c4.add_literal(make_literal(make_einstein_prop(c=Color.white, h=4), negated=True))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=3)))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=2)))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=1)))
    axioms.append(c4)

    c4 = Clause()
    c4.add_literal(make_literal(make_einstein_prop(c=Color.white, h=5), negated=True))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=4)))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=3)))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=2)))
    c4.add_literal(make_literal(make_einstein_prop(c=Color.green, h=1)))
    axioms.append(c4)

    # Green house's owner drinks coffee
    c5 = Clause()
    c5.add_literal(make_literal(make_einstein_prop(c=Color.green, b=Beverage.coffee)))
    axioms.append(c5)
    
    # Person who smokes Pall Mall drinks coffee
    c6 = Clause()
    c6.add_literal(make_literal(make_einstein_prop(s=Cigar.pallmall, p=Pet.birds)))
    axioms.append(c6)

    # The owner of the yellow house smokes Dunhill
    c7 = Clause()
    c7.add_literal(make_literal(make_einstein_prop(c=Color.yellow, s=Cigar.dunhill)))
    axioms.append(c7)

    # The man living in the center house drinks milk
    c8 = Clause()
    c8.add_literal(make_literal(make_einstein_prop(b=Beverage.milk, h=2)))
    axioms.append(c8)

    # The Norwegian lives in the first house
    c9 = Clause()
    c9.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=0)))
    axioms.append(c9)

    # The man who smokes Blends lives next to the one who keeps cats
    c10 = Clause()
    c10.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=0), negated=True))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=1)))
    axioms.append(c10)

    c10 = Clause()
    c10.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=1)))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=0), negated=True))
    axioms.append(c10)

    c10 = Clause()
    c10.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=4), negated=True))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=3)))
    axioms.append(c10)

    c10 = Clause()
    c10.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=3)))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=4), negated=True))
    axioms.append(c10)

    c10 = Clause()
    c10.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=1), negated=True))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=0)))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=2)))
    axioms.append(c10)

    c10 = Clause()
    c10.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=2), negated=True))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=1)))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=3)))
    axioms.append(c10)

    c10 = Clause()
    c10.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=3), negated=True))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=2)))
    c10.add_literal(make_literal(make_einstein_prop(p=Pet.cats, h=4)))
    axioms.append(c10)

    # The man who keeps the horse lives next to the man who smokes Dunhill
    c11 = Clause()
    c11.add_literal(make_literal(make_einstein_prop(p=Pet.horse, h=0), negated=True))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=1)))
    axioms.append(c11)

    c11 = Clause()
    c11.add_literal(make_literal(make_einstein_prop(p=Pet.horse, h=1)))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=0), negated=True))
    axioms.append(c11)

    c11 = Clause()
    c11.add_literal(make_literal(make_einstein_prop(p=Pet.horse, h=4), negated=True))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=3)))
    axioms.append(c11)

    c11 = Clause()
    c11.add_literal(make_literal(make_einstein_prop(p=Pet.horse, h=3)))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=4), negated=True))
    axioms.append(c11)

    c11 = Clause()
    c11.add_literal(make_literal(make_einstein_prop(p=Pet.horse, h=1), negated=True))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=0)))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=2)))
    axioms.append(c11)

    c11 = Clause()
    c11.add_literal(make_literal(make_einstein_prop(p=Pet.horse, h=2), negated=True))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=1)))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=3)))
    axioms.append(c11)

    c11 = Clause()
    c11.add_literal(make_literal(make_einstein_prop(p=Pet.horse, h=3), negated=True))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=2)))
    c11.add_literal(make_literal(make_einstein_prop(s=Cigar.dunhill, h=4)))
    axioms.append(c11)

    # The owner who smokes Bluemasters drinks beer
    c12 = Clause()
    c12.add_literal(make_literal(make_einstein_prop(b=Beverage.beer, s=Cigar.bluemasters)))
    axioms.append(c12)

    # The German smokes Prince
    c13 = Clause()
    c13.add_literal(make_literal(make_einstein_prop(n=Nationality.german, s=Cigar.prince)))
    axioms.append(c13)

    # The Norwegian lives next to the blue house
    c14 = Clause()
    c14.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=0), negated=True))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=1)))
    axioms.append(c14)

    c14 = Clause()
    c14.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=1)))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=0), negated=True))
    axioms.append(c14)

    c14 = Clause()
    c14.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=4), negated=True))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=3)))
    axioms.append(c14)

    c14 = Clause()
    c14.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=3)))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=4), negated=True))
    axioms.append(c14)

    c14 = Clause()
    c14.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=1), negated=True))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=0)))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=2)))
    axioms.append(c14)

    c14 = Clause()
    c14.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=2), negated=True))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=1)))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=3)))
    axioms.append(c14)

    c14 = Clause()
    c14.add_literal(make_literal(make_einstein_prop(n=Nationality.norwegian, h=3), negated=True))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=2)))
    c14.add_literal(make_literal(make_einstein_prop(c=Color.blue, h=4)))
    axioms.append(c14)

    # The man who smokes Blends has a neighbor who drinks water
    c15 = Clause()
    c15.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=0), negated=True))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=1)))
    axioms.append(c15)

    c15 = Clause()
    c15.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=1)))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=0), negated=True))
    axioms.append(c15)

    c15 = Clause()
    c15.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=4), negated=True))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=3)))
    axioms.append(c15)

    c15 = Clause()
    c15.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=3)))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=4), negated=True))
    axioms.append(c15)

    c15 = Clause()
    c15.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=1), negated=True))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=0)))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=2)))
    axioms.append(c15)

    c15 = Clause()
    c15.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=2), negated=True))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=1)))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=3)))
    axioms.append(c15)

    c15 = Clause()
    c15.add_literal(make_literal(make_einstein_prop(s=Cigar.blends, h=3), negated=True))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=2)))
    c15.add_literal(make_literal(make_einstein_prop(b=Beverage.water, h=4)))
    axioms.append(c15)

    return axioms


def run():
    assert(make_einstein_prop(n=Nationality.brit, c=Color.red) == 0)
    assert(make_einstein_prop(c=Color.red, b=Beverage.tea) == 125)
    assert(make_einstein_prop(b=Beverage.tea, s=Cigar.pallmall) == 225)
    assert(make_einstein_prop(s=Cigar.pallmall, p=Pet.dogs) == 300)
    assert(make_einstein_prop(p=Pet.dogs, h=0) == 350)
    assert(make_einstein_prop(p=Pet.fish, h=4) == 374)

    axioms = make_einstein_axioms()
    print("p cnf 375 {}".format(len(axioms)))
    for a in axioms:
        print(a)

if __name__ == "__main__":
    run()
