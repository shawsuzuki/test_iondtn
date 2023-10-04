/******************************************************************************
 **                           COPYRIGHT NOTICE
 **      (c) 2021 The Johns Hopkins University Applied Physics Laboratory
 **                         All rights reserved.
 ******************************************************************************/

/*****************************************************************************
 **
 ** File Name: sc_value.h
 **
 ** Namespace: bpsec_scv_
 **
 ** Description:
 **
 **     Security context values represent both the security parameters and
 **     security results associated with security blocks in a bundle.
 **
 **     There are four main types of values that are typically used when
 **     processing security contexts:
 **
 **     POLICY PARAMETERS:  A parameter that has been defined as part of the
 **                         policy configuration of the local node. Policy
 **                         parameters live in shared memory managed by the
 **                         ION Personal Space Manager (PSM).
 **
 **                         These parameters may be generated and used by
 **                         multiple threads within ION. For example, an admin
 **                         utility may generate policy parameters at ION
 **                         startup, a network management thread may accept
 **                         new policy parameters while the system is running,
 **                         and the main BPA thread may read these policy
 **                         parameters while generating/processing security
 **                         blocks.
 **
 **     WORKING PARAMETERS: A parameter that is created as part of security
 **                         processing solely in the context of a single
 **                         ION thread handling a security operation. These
 **                         parameters are neither stored in shared memory or
 **                         written to the SDR and are discarded as soon as
 **                         their associated processing is complete.
 **
 **                         An unwrapped session key is an example of such a
 **                         parameter. While a wrapped session key may be
 **                         present in a security block as a block parameter,
 **                         the unwrapped key is only used in the local memory
 **                         of an ION service using it to interact with a
 **                         cipher suite.
 **
 **                         Since these parameters are generated as-needed, and
 **                         in a single thread of control, they are allocated
 **                         on the heap using ION memory pools.
 **
 **
 **     BLOCK PARAMETERS:   A parameter that has been generated in the context
 **                         of processing a security block. These parameters
 **                         are either built from the encoding of parameter
 **                         values in a security block itself, or generated by
 **                         a single thread of control for the processing of a
 **                         single security block.
 **
 **                         When these parameters are associated with an
 **                         outgoing block, they are stored in the SDR with
 **                         other outgoing bundle information.
 **
 **                         When these parameters are associated with an
 **                         incoming bundle, they are stored in working
 **                         memory as part of processing security as
 **                         part of bundle acceptance. Once the incoming
 **                         bundle has been accepted, it is written to the
 **                         SDR (or delivered to the local application agent).
 **
 **     BLOCK RESULTS:      A security result associated with a security
 **                         operation within a security block.
 **
 **                         When results are associated with an outgoing
 **                         block, they are stored in the SDR with other
 **                         outgoing bundle information.
 **
 **                         When results are associated with an
 **                         incoming bundle, they are stored in working
 **                         memory as part of processing security as
 **                         part of bundle acceptance. Once the incoming
 **                         bundle has been accepted, it is written to the
 **                         SDR (or delivered to the local application agent).
 **
 ** Notes:
 **
 ** - Parameter handling for security contexts is complicated in ION because
 **   parameters can be stored in different areas of ION (such as working memory,
 **   shared memory, or the SDR). Therefore, SC values definitions contain
 **   additional annotations and functions to help with describing how these
 **   values are stored and where they are defined.
 **
 **
 ** Assumptions:
 **
 ** Modification History:
 **  MM/DD/YY  AUTHOR         DESCRIPTION
 **  --------  ------------   ---------------------------------------------
 **  02/18/22  E. Birrane     Initial implementation
 **
 *****************************************************************************/

#ifndef _SC_VALUE_H_
#define _SC_VALUE_H_

#include "platform.h"
#include "ion.h"
#include "psm.h"
#include "smlist.h"

#include "sci_structs.h"
#include "sci.h"



/*****************************************************************************
 *                           CONSTANTS DEFINITIONS                           *
 *****************************************************************************/




/*****************************************************************************
 *                                Structures                                 *
 *****************************************************************************/


// TODO: Do we assume we are always using the ION partition and ION SDR, or do we
//       keep it parameterized. At least when parameterized it makes it clear that
//       the function is for SM or SDR.

/** General Value Functions. **/
void       bpsec_scv_clear(PsmPartition wm, sc_value *val); // bpsec_sciutil_valMemRelease
void*      bpsec_scv_rawAlloc(PsmPartition wm, sc_value *val, int length); // was bpsec_sciutil_valDataALloc
void*      bpsec_scv_rawGet(PsmPartition wm, sc_value *val);


/** Lyst Processing Functions. **/
void       bpsec_scv_lystCbDel(LystElt elt, void *arg); // was bpsec_sciutil_valLystCbDel
sc_value*  bpsec_scv_lystFind(Lyst list, int id, int type); // was sciMemValListFind


/**  SC_VAL_STORE_MEM Functions **/
sc_value*  bpsec_scv_memCopy(sc_value *oldVal); // was bpsec_sciutil_valMemCopy
int        bpsec_scv_memCreate(sc_value *val, sc_val_type type, int id, int length); // was bpsec_sciutil_valMemCreate
sc_value   bpsec_scv_memCsiConvert(csi_val_t csi_value, int type, int id);
sc_value*  bpsec_scv_memDeserialize(int sc_id, int val_type, unsigned char **cursor, unsigned int *unparsedBytes); // was bpsec_sci_valDeserialize
Object     bpsec_scv_memListRecord(Sdr sdr, Object list, Lyst values); // was bpsec_sci_valListRecord
int        bpsec_scv_memSerialize(sc_Def *def, sc_value *val, uint8_t **resultData, unsigned int *resultLen);
Object     bpsec_scv_memSdrConvert(Sdr sdr, sc_value *oldVal); // was bpsec_sci_valSdrRecord
int        bpsec_scv_memSdrListAppend(Sdr sdr, Object list, sc_value *val); // was sciuSdrValListAppend and bpsec_sci_valSdrListAppend


/**  SC_VAL_STORE_SDR Functions **/
Object    bpsec_scv_sdrCopy(Sdr sdr, Object oldObj); // was bpsec_sci_valSdrCopy
void      bpsec_scv_sdrListCbDel(Sdr sdr, Object eltData, void *args);
Object    bpsec_scv_sdrListCopy(Sdr sdr, Object oldList); // was bpsec_sci_valSdrListCopy
Lyst      bpsec_scv_sdrListRead(Sdr sdr, Object list); // was bpsec_sci_playbackSciValList
uint8_t*  bpsec_scv_sdrListSerialize(Sdr sdr, sc_Def *def, Object list, unsigned int *length); // was bpsec_sci_valSdrListSerialize
sc_value* bpsec_scv_sdrMemConvert(Sdr sdr, Object valAddr); //was bpsec_sci_playbackSciVal
void      bpsec_scv_sdrRelease(Sdr sdr, Object val); // was bpsec_sci_valSdrDestroy
int       bpsec_scv_sdrSerialize(Sdr sdr, sc_Def *def, Object valObj, uint8_t **data, unsigned int *length);




/**  SC_VAL_STORE_SM Functions **/
PsmAddress bpsec_scv_smCreate(PsmPartition wm, sc_val_type type); // was bpsec_sciutil_valSharedCreate
void       bpsec_scv_smlistCbDel(PsmPartition partition, PsmAddress elt, void *arg); // was bpsec_sciutil_valSmlistCbDel
char*      bpsec_scv_smListPrint(PsmPartition wm, sc_Def *sc_def, Lyst vals); // was bpsec_sciutil_valsPrint
Object     bpsec_scv_smListRecord(Sdr sdr, Object list, PsmPartition wm, PsmAddress values); // was bpsec_sci_recordSciValSharedList
Object     bpsec_scv_smSdrConvert(Sdr sdr, PsmPartition wm, PsmAddress oldValAddr); // was bpsec_sci_recordSharedSciVal





#endif // SC_VALUE_H
