// Card.h
#ifndef CARD_H
#define CARD_H

#include "Utils.h"

typedef char Color;   // 'R','Y','G','B','W'
typedef char Value;   // '0'-'9','S','R','D','W','4'

class Card {
private:
    Color color;
    Value value;
    static int cardsCreated;

    static int colorIndex(Color c);
    static int valueRank(const Card& c);

public:
    Card();
    Card(Color c, Value v);

    Color getColor() const { return color; }
    Value getValue() const { return value; }

    bool isWild() const;
    bool isWildDrawFour() const;
    bool isNumber() const;
    bool isAction() const;
    bool matches(const Card& top, Color activeColor) const;
    string toString() const;

    bool operator==(const Card& o) const;
    bool operator<(const Card& o) const;

    static const char* colorName(Color c);
    static int getCardsCreated();

    friend ostream& operator<<(ostream& os, const Card& c);
};

#endif
