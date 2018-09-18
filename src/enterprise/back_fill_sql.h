#ifndef BACK_FILL_SQL_H
#define BACK_FILL_SQL_H

class BackFillSql {
    const int m_current_height;
    const int m_back_fill_depth;

public:
    BackFillSql(const int current_height, const int back_fill_depth);

};


#endif //BACK_FILL_SQL_H