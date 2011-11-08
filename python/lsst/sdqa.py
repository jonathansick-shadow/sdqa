"""Implementation of sdqa in pure python

Doubtless incomplete, but probably enough to tide us over until we finish getting rid of sdqa
"""

class SdqaRating(object):
    INVALID = -1
    AMP = 0
    CCD = 1
    FPA = 2
    FOOTPRINT = 3

    def __init__(self, name, value, error, scope):
        self.name = name
        self.value = value
        self.error = error
        self.scope = scope

    def getError(self):
        return self.error

    def getName(self):
        return self.name

    def getScope(self):
        return self.scope

    def getValue(self):
        return self.value

class SdqaRatingSet(object):
    def __init__(self):
        self.ratings = []

    def append(self, rating):
        self.ratings.append(rating)

    def __iter__(self):
        self.__index = len(self.ratings)
        return self

    def next(self):
        if self.__index == 0:
            raise StopIteration

        self.__index -= 1
        return self.ratings[self.__index]
