#include "openning.h"

const Move a2a3 = MOVE(SQ_A2, SQ_A3);
const Move a2a4 = MOVE(SQ_A2, SQ_A4);
const Move a7a6 = MOVE(SQ_A7, SQ_A6);

const Move b1a3 = MOVE(SQ_B1, SQ_A3);
const Move b1c3 = MOVE(SQ_B1, SQ_C3);
const Move b2b3 = MOVE(SQ_B2, SQ_B3);
const Move b2b4 = MOVE(SQ_B2, SQ_B4);
const Move b7b6 = MOVE(SQ_B7, SQ_B6);
const Move b8c6 = MOVE(SQ_B8, SQ_C6);

const Move c2c3 = MOVE(SQ_C2, SQ_C3);
const Move c2c4 = MOVE(SQ_C2, SQ_C4);
const Move c7c5 = MOVE(SQ_C7, SQ_C5);
const Move c7c6 = MOVE(SQ_C7, SQ_C6);

const Move d2d3 = MOVE(SQ_D2, SQ_D3); 
const Move d2d4 = MOVE(SQ_D2, SQ_D4); 
const Move d7d6 = MOVE(SQ_D7, SQ_D6);
const Move d7d5 = MOVE(SQ_D7, SQ_D5);

const Move e2e3 = MOVE(SQ_E2, SQ_E3);
const Move e2e4 = MOVE(SQ_E2, SQ_E4);
const Move e7e5 = MOVE(SQ_E7, SQ_E5);
const Move e7e6 = MOVE(SQ_E7, SQ_E6);

const Move f2f3 = MOVE(SQ_F2, SQ_F4);
const Move f2f4 = MOVE(SQ_F2, SQ_F4);
const Move f7f5 = MOVE(SQ_F7, SQ_F5);

const Move g1f3 = MOVE(SQ_G1, SQ_F3);
const Move g1h3 = MOVE(SQ_G1, SQ_H3);
const Move g2g3 = MOVE(SQ_G2, SQ_G3);
const Move g2g4 = MOVE(SQ_G2, SQ_G3);
const Move g7g5 = MOVE(SQ_G7, SQ_G5);
const Move g7g6 = MOVE(SQ_G7, SQ_G6);
const Move g8f6 = MOVE(SQ_G8, SQ_F6);

const Move h2h3 = MOVE(SQ_H2, SQ_H3);
const Move h2h4 = MOVE(SQ_H2, SQ_H4);


//  0 Ply
//  Start Position
const Openning A00_Start_position = 
    { "A00  Start position",       NULL_MOVE };

//  1 Ply
//  from A00  Start position
const Openning B00_Kings_Pawn =  
    { "B00  Kings Pawn",           e2e4, NULL_MOVE };
const Openning A40_Queens_Pawn =  
    { "A40  Queens Pawn",          d2d4, NULL_MOVE };
const Openning A04_Reti =  
    { "A04  Reti",                 g1f3, NULL_MOVE };
const Openning A10_English =  
    { "A10  English",              c2c4, NULL_MOVE };
const Openning A00_Benko_Opening =  
    { "A00  Benko Opening",        g2g3, NULL_MOVE };
const Openning A00_Van_Geet =  
    { "A00  Van Geet (Dunst)",     b1c3, NULL_MOVE };
const Openning A00_Van_Kruijs =  
    { "A00  Van Kruijs",           e2e3, NULL_MOVE };
const Openning A02_Bird =  
    { "A02  Bird",                 f2f4, NULL_MOVE };
const Openning A01_Nimzowitsch_Larsen_Attack =  
    { "A01  Nimzowitsch-Larsen Attack", b2b3, NULL_MOVE };
const Openning A00_Mieses =  
    { "A00  Mieses",               d2d3, NULL_MOVE };
const Openning A00_Saragossa =
    { "A00  Saragossa",            c2c3, NULL_MOVE };
const Openning A00_Polish = 
    { "A00  Polish (Sokolsky; Orang-Utan)", b2b4, NULL_MOVE };
const Openning A00_Grob = 
    { "A00  Grob",                 g2g4, NULL_MOVE };
const Openning A00_Barnes_Opening = 
    { "A00  Barnes Opening",       f2f3, NULL_MOVE };
const Openning A00_Kadas_Opening =
    { "A00  Kadas Opening",        h2h4, NULL_MOVE };
const Openning A00_Anderssen_Opening =
    { "A00  Anderssen Opening",    a2a3, NULL_MOVE };
const Openning A00_Clemenz_Opening =
    { "A00  Clemenz Opening",      h2h3, NULL_MOVE };
const Openning A00_Ware_Opening =
    { "A00  Ware Opening",         a2a4, NULL_MOVE };
const Openning A00_Amar_Paris_Opening =
    { "A00  Amar/Paris Opening",   g1h3, NULL_MOVE };
const Openning A00_Durkin =
    { "A00  Durkin",               b1a3, NULL_MOVE };

//  2 Ply
//  from B00  Kings Pawn
const Openning B20_Sicilian_Defence = 
    { "B20  Sicilian Defence",     e2e4, c7c5, NULL_MOVE };
const Openning C20_Open_Game =  
    { "C20  Open Game",            e2e4, e7e5, NULL_MOVE };
const Openning C00_French =  
    { "C00  French",               e2e4, e7e6, NULL_MOVE };
const Openning B10_Caro_Kann =  
    { "B10  Caro-Kann",            e2e4, c7c6, NULL_MOVE };
const Openning B01_Scandinavian =  
    { "B01  Scandinavian",         e2e4, d7d5, NULL_MOVE };
const Openning B07_Pirc =  
    { "B07  Pirc",                 e2e4, d7d6, NULL_MOVE };
const Openning B06_Modern =  
    { "B06  Modern",               e2e4, g7g6, NULL_MOVE };
const Openning B02_Alekhine_Defence =  
    { "B02  Alekhine Defence",     e2e4, g8f6, NULL_MOVE };
const Openning B00_Nimzowitsch_Defence =
    { "B00  Nimzowitsch Defence",  e2e4, b8c6, NULL_MOVE };
const Openning B00_Owen_Defence =
    { "B00  Owen Defence",         e2e4, b7b6, NULL_MOVE };
const Openning B00_St_George_Defence =
    { "B00  St. George Defence",   e2e4, a7a6, NULL_MOVE };
const Openning Kings_Pawn_Fred =
    { "Kings Pawn: Fred",          e2e4, f7f5, NULL_MOVE };
const Openning Reversed_Grob =
    { "Reversed Grob (Borg/Basman Defence)", e2e4, g7g5, NULL_MOVE };

//  2 Ply
//  from A40  Queens Pawn





