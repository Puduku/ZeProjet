// c-ansi/int-types-rtt.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: simple tests @ runtime on int types
// =======


#ifndef __C_ANSI_INT_RUNTIME_TEST_H_INCLUDED__
#define __C_ANSI_INT_RUNTIME_TEST_H_INCLUDED__

#include "c-ansi/types.h"
#include "c-ansi/stderr.h"


//
#define b_SIGNED(m_intType) (((m_intType) -1) < 0)

//
#define b_DIFFERENT_SIGNEDNESS(m_intType1,m_intType2) \
(b_SIGNED(m_intType1)? !b_SIGNED(m_intType2): b_SIGNED(m_intType2))

//
#define m_CHECK_SAME_SIGNEDNESS(m_intType1,m_intType2) \
m_RAISE_IF(b_DIFFERENT_SIGNEDNESS(m_intType1,m_intType2),\
           #m_intType1 " and " #m_intType2 " have different signedness")

//
#define m_CHECK_UNSIGNED(m_intType) \
m_RAISE_IF(b_SIGNED(m_intType), #m_intType " is signed")

//
#define m_CHECK_SIGNED(m_intType) \
m_RAISE_IF(!b_SIGNED(m_intType), #m_intType " is not signed")

//
#define m_CHECK_SAME_SIZE(m_intType1,m_intType2) \
m_RAISE_IF(sizeof(m_intType1) != sizeof(m_intType2),\
           "sizeof(" #m_intType1 ")=%u != sizeof(" #m_intType2 ")=%u",sizeof(m_intType1),sizeof(m_intType2))

//
#define m_CHECK_COMPARISON(m_intType,intExpr1,m_op,intExpr2) \
m_RAISE_IF(!((m_intType)intExpr1 m_op (m_intType)intExpr2),\
           "WRONG ASSERTION: " #intExpr1 " " #m_op " " #intExpr2,intExpr1,intExpr2)

//
#define m_CHECK_EQUALITY(m_intType,intExpr1,intExpr2) \
m_CHECK_COMPARISON(m_intType,intExpr1,==,intExpr2)





#endif // __C_ANSI_INT_RUNTIME_TEST_H_INCLUDED__

