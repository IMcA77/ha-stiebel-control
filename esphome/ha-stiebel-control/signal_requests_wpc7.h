/*
 * Signal Request Table — WPC7 / Stiebel WPC-07cool / Tecalor TTF07c
 *
 * Community-tested on real WPC7 hardware (PR #27). Ported from community project
 * kr0ner/OneESP32ToRuleThemAll (CAN register map for their TTF07c model),
 * re-implemented against this project's ElsterTable.h/signal_requests
 * conventions — no code or YAML was copied verbatim. See issue #2 and #21
 * for community thread. Please test against a real WPC7/TTF07c unit and
 * report back (or submit corrections) before relying on this for control.
 *
 * CAN targets confirmed from the source project: every TTF07c signal is
 * queried from either Kessel (0x180) or HK1 (0x301, our BEDIENMODUL_2) —
 * no new CanMembers entries were needed for this model.
 *
 * Known gaps (see docs/CONTRIBUTING.md "mark unverified signals"):
 * - VERDICHTER (compressor power, base signal) is queried against
 *   cm_heizmodul (0x500) in SIGNAL_REQUESTS_BASE, which doesn't exist as a
 *   physical node on WPC7. The source project instead derives a boolean
 *   "compressor running" from bit 3 of BETRIEBS_STATUS_WPC7 — a different
 *   signal entirely. No direct compressor-power equivalent is known for
 *   WPC7 yet; BETRIEBS_STATUS_WPC7 is requested below as a raw bitmask for
 *   future per-bit decoding.
 * - The 6 base energy-counter signals (EL_AUFNAHMELEISTUNG_*, WAERMEERTRAG_*)
 *   are also targeted at cm_heizmodul in the base set and likely won't
 *   respond on WPC7 either; COP sensors will probably stay unpublished
 *   until this is verified/fixed by someone with real hardware.
 * - A few signals below reuse existing ElsterTable.h rows whose Type
 *   differs from what the source project documents for TTF07c (index
 *   matches, type doesn't) — flagged inline. The existing Type was kept
 *   as-is since other models may depend on it; values may be mis-scaled.
 */

#ifndef SIGNAL_REQUESTS_WPC7_H
#define SIGNAL_REQUESTS_WPC7_H

#include "config.h"
#include "signal_requests_base.h"

extern const SignalRequest signalRequests[] = {
    SIGNAL_REQUESTS_BASE   // date/time, EVU lock, operating mode

    // ========================================================================
    // KESSEL (0x180): Live Temperatures, Flow, Pressures & Pumps
    // ========================================================================
    {"FEHLERMELDUNG",              FREQ_1MIN,  cm_kessel},  // 0x0001
    {"SPEICHERSOLLTEMP",           FREQ_30S,   cm_kessel},  // 0x0003
    {"AUSSENTEMP",                 FREQ_30S,   cm_kessel},  // 0x000c
    {"SPEICHERISTTEMP",            FREQ_30S,   cm_kessel},  // 0x000e
    {"RUECKLAUFISTTEMP",           FREQ_30S,   cm_kessel},  // 0x0016
    {"HZK_PUMPE",                  FREQ_30S,   cm_kessel},  // 0x0053
    {"HEIZKREIS_STATUS",           FREQ_30S,   cm_kessel},  // 0x0059
    {"SPEICHER_STATUS",            FREQ_30S,   cm_kessel},  // 0x005a
    {"ABTAUUNGAKTIV",              FREQ_1MIN,  cm_kessel},  // 0x0061
    {"QUELLE_IST",                 FREQ_30S,   cm_kessel},  // 0x01d4
    {"WPVORLAUFIST",               FREQ_30S,   cm_kessel},  // 0x01d6
    {"HILFSKESSELSOLL",            FREQ_1MIN,  cm_kessel},  // 0x01d7
    {"MAX_HEIZUNG_TEMP",           FREQ_10MIN, cm_kessel},  // 0x01e8
    {"HEISSGAS_TEMP",              FREQ_30S,   cm_kessel},  // 0x0265
    {"MASCHINENDRUCK",             FREQ_30S,   cm_kessel},  // 0x0268
    {"VOLUMENSTROM_WPC7",          FREQ_30S,   cm_kessel},  // 0x0673
    {"DRUCK_HEIZKREIS",            FREQ_1MIN,  cm_kessel},  // 0x0674
    {"QUELLENDRUCK",               FREQ_1MIN,  cm_kessel},  // 0x0675
    {"VORLAUFISTTEMP_WPC7",        FREQ_30S,   cm_kessel},  // 0x06a1
    {"KUEHLEN_EINGESCHALTET",      FREQ_1MIN,  cm_kessel},  // 0x06aa
    {"LEISTUNG_HEIZKREISPUMPE",    FREQ_1MIN,  cm_kessel},  // 0x070a
    {"LEISTUNG_WARMWASSERPUMPE",   FREQ_1MIN,  cm_kessel},  // 0x070b
    {"LEISTUNG_SOLEPUMPE",         FREQ_1MIN,  cm_kessel},  // 0x070c
    {"EINPHASIGER_BETRIEB",        FREQ_10MIN, cm_kessel},  // 0x070e
    {"MESSSTROM_HOCHDRUCK",        FREQ_1MIN,  cm_kessel},  // 0x07a4
    {"MESSSTROM_NIEDERDRUCK",      FREQ_1MIN,  cm_kessel},  // 0x07a5
    {"AUSSEN_FROSTTEMP",           FREQ_10MIN, cm_kessel},  // 0x0a00
    {"WP_PUMPENSTATUS",            FREQ_30S,   cm_kessel},  // 0xfdad
    {"WP_STATUS",                  FREQ_30S,   cm_kessel},  // 0xfdae

    // ========================================================================
    // KESSEL (0x180): Runtimes & Energy Counters (enables COP calculations)
    // ========================================================================
    {"LAUFZEIT_DHC1",              FREQ_10MIN, cm_kessel},  // 0x0259
    {"LAUFZEIT_DHC2",              FREQ_10MIN, cm_kessel},  // 0x025a
    {"LZ_VERD_1_HEIZBETRIEB",      FREQ_10MIN, cm_kessel},  // 0x07fc
    {"LZ_VERD_1_WW_BETRIEB",       FREQ_10MIN, cm_kessel},  // 0x0802
    {"LZ_DHC12",                   FREQ_10MIN, cm_kessel},  // 0x0805
    {"EL_AUFNAHMELEISTUNG_WW_TAG_WH",    FREQ_10MIN, cm_kessel},  // 0x091a
    {"EL_AUFNAHMELEISTUNG_WW_TAG_KWH",   FREQ_10MIN, cm_kessel},  // 0x091b
    {"EL_AUFNAHMELEISTUNG_WW_SUM_KWH",   FREQ_10MIN, cm_kessel},  // 0x091c
    {"EL_AUFNAHMELEISTUNG_WW_SUM_MWH",   FREQ_10MIN, cm_kessel},  // 0x091d
    {"EL_AUFNAHMELEISTUNG_HEIZ_TAG_WH",  FREQ_10MIN, cm_kessel},  // 0x091e
    {"EL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH", FREQ_10MIN, cm_kessel},  // 0x091f
    {"EL_AUFNAHMELEISTUNG_HEIZ_SUM_KWH", FREQ_10MIN, cm_kessel},  // 0x0920
    {"EL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH", FREQ_10MIN, cm_kessel},  // 0x0921
    {"WAERMEERTRAG_2WE_WW_TAG_WH",       FREQ_10MIN, cm_kessel},  // 0x0922
    {"WAERMEERTRAG_2WE_WW_TAG_KWH",      FREQ_10MIN, cm_kessel},  // 0x0923
    {"WAERMEERTRAG_2WE_WW_SUM_KWH",      FREQ_10MIN, cm_kessel},  // 0x0924
    {"WAERMEERTRAG_2WE_WW_SUM_MWH",      FREQ_10MIN, cm_kessel},  // 0x0925
    {"WAERMEERTRAG_2WE_HEIZ_TAG_WH",     FREQ_10MIN, cm_kessel},  // 0x0926
    {"WAERMEERTRAG_2WE_HEIZ_TAG_KWH",    FREQ_10MIN, cm_kessel},  // 0x0927
    {"WAERMEERTRAG_2WE_HEIZ_SUM_KWH",    FREQ_10MIN, cm_kessel},  // 0x0928
    {"WAERMEERTRAG_2WE_HEIZ_SUM_MWH",    FREQ_10MIN, cm_kessel},  // 0x0929
    {"WAERMEERTRAG_WW_TAG_WH",           FREQ_10MIN, cm_kessel},  // 0x092a
    {"WAERMEERTRAG_WW_TAG_KWH",          FREQ_10MIN, cm_kessel},  // 0x092b
    {"WAERMEERTRAG_WW_SUM_KWH",          FREQ_10MIN, cm_kessel},  // 0x092c
    {"WAERMEERTRAG_WW_SUM_MWH",          FREQ_10MIN, cm_kessel},  // 0x092d
    {"WAERMEERTRAG_HEIZ_TAG_WH",         FREQ_10MIN, cm_kessel},  // 0x092e
    {"WAERMEERTRAG_HEIZ_TAG_KWH",        FREQ_10MIN, cm_kessel},  // 0x092f
    {"WAERMEERTRAG_HEIZ_SUM_KWH",        FREQ_10MIN, cm_kessel},  // 0x0930
    {"WAERMEERTRAG_HEIZ_SUM_MWH",        FREQ_10MIN, cm_kessel},  // 0x0931

    // ========================================================================
    // MANAGER (WPM): Setpoints, Heating Curve & Grid/Auxiliary States
    // ========================================================================
    {"RAUMSOLLTEMP_I",             FREQ_1MIN,  cm_manager},  // 0x0005
    {"RAUMSOLLTEMP_II",            FREQ_10MIN, cm_manager},  // 0x0006
    {"RAUMSOLLTEMP_III",           FREQ_10MIN, cm_manager},  // 0x0007
    {"RAUMSOLLTEMP_NACHT",         FREQ_1MIN,  cm_manager},  // 0x0008
    {"EINSTELL_SPEICHERSOLLTEMP",  FREQ_30S,   cm_manager},  // 0x0013
    {"ANTILEGIONELLEN",            FREQ_10MIN, cm_manager},  // 0x0101
    {"HEIZKURVE",                  FREQ_1MIN,  cm_manager},  // 0x010e
    {"QUELLENSOLLTEMPERATUR",      FREQ_10MIN, cm_manager},  // 0x01b0
    {"WW_ECO",                     FREQ_10MIN, cm_manager},  // 0x027e
    {"FATAL_ERROR",                FREQ_1MIN,  cm_manager},  // 0x080e
    {"EINSTELL_SPEICHERSOLLTEMP2", FREQ_30S,   cm_manager},  // 0x0a06
    {"ZWEITER_WE_STATUS",          FREQ_1MIN,  cm_manager},  // 0xfdab
    {"WP_EVU",                     FREQ_1MIN,  cm_manager},  // 0xfdac

    // ========================================================================
    // BEDIENMODUL_2 (0x301): Room & Flow Setpoint Entities (if present)
    // ========================================================================
    {"RAUMISTTEMP",                FREQ_1MIN,  cm_bedienmodul_2},
    {"VERSTELLTE_RAUMSOLLTEMP",    FREQ_1MIN,  cm_bedienmodul_2},
    {"VORLAUFSOLLTEMP",            FREQ_1MIN,  cm_bedienmodul_2},
    {"RAUMFEUCHTE",                FREQ_10MIN, cm_bedienmodul_2},
};

extern const size_t SIGNAL_REQUEST_COUNT_VALUE = sizeof(signalRequests) / sizeof(SignalRequest);

#endif // SIGNAL_REQUESTS_WPC7_H
