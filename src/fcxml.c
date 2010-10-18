/*
 * fontconfig/src/fcxml.c
 *
 * Copyright © 2002 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the author(s) not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors make no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE AUTHOR(S) DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "fcint.h"

static void
FcExprDestroy (FcExpr *e);

void
FcTestDestroy (FcTest *test)
{
    if (test->next)
        FcTestDestroy (test->next);
    FcExprDestroy (test->expr);
    FcMemFree (FC_MEM_TEST, sizeof (FcTest));
    free (test);
}

static void
FcExprDestroy (FcExpr *e)
{
    if (!e)
        return;
    switch (e->op) {
    case FcOpInteger:
        break;
    case FcOpDouble:
        break;
    case FcOpString:
        break;
    case FcOpMatrix:
        FcMatrixFree (e->u.mval);
        break;
    case FcOpCharSet:
        FcCharSetDestroy (e->u.cval);
        break;
    case FcOpBool:
        break;
    case FcOpField:
        break;
    case FcOpConst:
        break;
    case FcOpAssign:
    case FcOpAssignReplace:
    case FcOpPrepend:
    case FcOpPrependFirst:
    case FcOpAppend:
    case FcOpAppendLast:
        break;
    case FcOpOr:
    case FcOpAnd:
    case FcOpEqual:
    case FcOpNotEqual:
    case FcOpLess:
    case FcOpLessEqual:
    case FcOpMore:
    case FcOpMoreEqual:
    case FcOpContains:
    case FcOpListing:
    case FcOpNotContains:
    case FcOpPlus:
    case FcOpMinus:
    case FcOpTimes:
    case FcOpDivide:
    case FcOpQuest:
    case FcOpComma:
        FcExprDestroy (e->u.tree.right);
        /* fall through */
    case FcOpNot:
    case FcOpFloor:
    case FcOpCeil:
    case FcOpRound:
    case FcOpTrunc:
        FcExprDestroy (e->u.tree.left);
        break;
    case FcOpNil:
    case FcOpInvalid:
        break;
    }

    e->op = FcOpNil;
}

void
FcEditDestroy (FcEdit *e)
{
    if (e->next)
        FcEditDestroy (e->next);
    if (e->expr)
        FcExprDestroy (e->expr);
    free (e);
}

FcBool
FcConfigParseAndLoadDir (FcConfig        *config,
                         const FcChar8        *name,
                         const FcChar8        *dir,
                         FcBool                complain)
{
    return FcConfigParseAndLoad (config, name, complain);
}
