#ifndef TRANSFORMHISTORY_H
#define TRANSFORMHISTORY_H

#include <QString>

class TransformHistory
{
public:
    TransformHistory();
    typedef struct blur_effect {
        QString type;
        int kernel;
    } blur_effect;
};

#endif // TRANSFORMHISTORY_H
