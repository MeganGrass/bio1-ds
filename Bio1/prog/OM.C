/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OM.C
 */

#include "TYPES.H"
#include "OM.H"
#include "OMA.H"
#include "SCA.H"
#include "SCE0.H"
#include "SCE_AT.H"
#include "SCENARIO.H"
#include "FLAG.H"

VOID Om_trans(VOID)
{
    OBJ_MODEL_WORK* pOm;
    UCHAR i;
    UCHAR j;
    UCHAR n;

    for (i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            n = G.pRoom->nOmodel;
        }
        else
        {
            n = G.pRoom->nItem;
        }
        for (j = 0; j < n; j++)
        {
            if (i == 0)
            {
                pOm = Omodel_tbl[j];
            }
            else
            {
                pOm = pOm_tbl[j];
            }
            if (pOm != 0 && (pOm->Be_flg & OM_ENABLED) && pOm->pTmd != 0)
            {
                RotMatrix((SVECTOR*)(&pOm->Cdir_x), &pOm->Coord);  // reinterprets the memory as SVECTOR
                pOm->Coord_flg = 0;
                Om_obj_trans(pOm);
            }
        }
    }
}

INT Om_move(VOID)
{
    OBJ_MODEL_WORK* pOm;
    OBJ_MODEL_WORK* pOm2;
    ENEMY_WORK* pEm;
    INT i;
    INT k;
    INT n;

    INT Hit_cnt;

    INT Bak_pos_x;
    INT Bak_pos_z;

    i = 0;

    Hit_cnt = 0;

    for (; i < G.pRoom->nOmodel; i++)
    {
        pOm = Omodel_tbl[i];
        if (pOm == 0 || !(pOm->Be_flg & OM_ENABLED))
        {
            continue;
        }
        n = G.Enemy_num;
        for (pEm = MAIN.Enemy; n > 0; pEm++)
        {
            if (pEm->Be_flg & EM_BE_ENABLED)
            {
                n--;
                Oma_ck_oba(pEm, pOm, 0);
            }
        }
        Bak_pos_x = pOm->Coord.t[0];
        Bak_pos_z = pOm->Coord.t[2];

        if (Oma_ck_oba(PL_EM, pOm, 1) != 0 && (G.SaveData.Key & KEY_UP) && Oma_ck_front(pOm) != 0)
        {
            pOm->Push_cnt++;
        }
        else
        {
            pOm->Push_cnt = 0;
        }

        if (!(pOm->Be_flg & 0x20) && pOm->Push_cnt == 9)
        {
            G.pEm = (ENEMY_WORK*)pOm;  // reinterprets the memory as ENEMY_WORK
            if (!(pOm->Be_flg & 4) && FN_CAST(INT, (VOID*, VOID*), Sca_ck_point2)(&pOm->Probe1, &pOm->Probe2) != 0)
            {
            fail:
                pOm->Push_cnt = 10;

                pPush_om = 0;

                goto restore;
            }

            Hit_cnt = 1;

            pEm = MAIN.Enemy;

            n = G.Enemy_num;

            pOm->Push_cnt = 8;
            pPush_om = (ENEMY_WORK*)pOm;  // reinterprets the memory as ENEMY_WORK
            G.Player.Cdir_y = (G.Player.Cdir_y + 0x200) & 0xC00;

            for (; n > 0; pEm++)
            {
                if (pEm->Be_flg & EM_BE_ENABLED)
                {
                    n--;
                    if (Oma_ck_oba(pEm, pOm, 1) != 0)
                    {
                        pOm->Push_cnt = 10;
                    }
                }
            }

            for (k = 0; k < G.pRoom->nOmodel; k++)
            {
                pOm2 = Omodel_tbl[k];
                if ((pOm2->Be_flg & OM_ENABLED) && pOm != pOm2 && Oma_ck_om(pOm2, pOm) != 0)
                {
                    goto fail;
                }
            }
            if (G.Player.Damage_no != 0 || G.Player.Routine_2 == 0x10)
            {
                goto skip;
            }
            STOP_FLG_CUR &= ~STOP_ACTION;
        }

    restore:
        pOm->Coord.t[0] = Bak_pos_x;
        pOm->Coord.t[2] = Bak_pos_z;
    skip:
        Oma_ck_oba(PL_EM, pOm, 0);
        if (pOm->Coord.t[0] != pOm->Old_x || pOm->Coord.t[2] != (SHORT)pOm->Old_z)  // sign-extends the halfword
        {
            for (k = 0; k < G.pRoom->nOmodel; k++)
            {
                pOm2 = Omodel_tbl[k];
                if ((pOm2->Be_flg & OM_ENABLED) && pOm != pOm2)
                {
                    Oma_ck_om(pOm, pOm2);
                }
            }
            pOm->Old_x = pOm->Coord.t[0];
            pOm->Old_z = pOm->Coord.t[2];
        }
        Sce_at_check((ENEMY_WORK*)pOm, SCE_04);  // reinterprets the memory as ENEMY_WORK
    }

    if (Hit_cnt != 0)
    {
        MAIN.System_flg |= SYS_PUSH_OBJ;
#ifdef __INTELLISENSE__
        return 0;  // value-less return in an INT function is GCC-only
#else
        return;
#endif
    }
    if (MAIN.System_flg & SYS_PUSH_OBJ)
    {
        MAIN.System_flg &= ~SYS_PUSH_OBJ;
        STOP_FLG_CUR |= STOP_ACTION;
    }
}
