// *****************************************************************************
// *****************************************************************************
// Copyright 2013, Cadence Design Systems
//
// This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
// Distribution,  Product Version 5.8.
//
// Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
//    implied. See the License for the specific language governing
//    permissions and limitations under the License.
//
// For updates, support, or to become part of the LEF/DEF Community,
// check www.openeda.org for details.
//
//  $Author: dell $
//  $Revision: #1 $
//  $Date: 2020/09/29 $
//  $State:  $
// *****************************************************************************
// *****************************************************************************

/*
 * User header file for the DEF Interface.  This includes
 * all of the header files which are relevant to both the
 * reader and the writer.
 *
 * defrReader.h and defwWriter.h include this file, so that
 * an application only needs to include either defwReader.h
 * or defwWriter.h.
 */

#ifndef DEFI_USER_H
#define DEFI_USER_H

/* General utilities. */
/* #include "defiMalloc.hpp" */
/* #include "defiUtils.hpp" */

/*
 * API objects
 */
#include "defiAssertion.hpp"
#include "defiBlockage.hpp"
#include "defiComponent.hpp"
#include "defiDebug.hpp"
#include "defiFPC.hpp"
#include "defiFill.hpp"
#include "defiGroup.hpp"
#include "defiIOTiming.hpp"
#include "defiNet.hpp"
#include "defiNonDefault.hpp"
#include "defiPartition.hpp"
#include "defiPath.hpp"
#include "defiPinCap.hpp"
#include "defiPinProp.hpp"
#include "defiProp.hpp"
#include "defiPropType.hpp"
#include "defiRegion.hpp"
#include "defiRowTrack.hpp"
#include "defiScanchain.hpp"
#include "defiSite.hpp"
#include "defiSlot.hpp"
#include "defiTimingDisable.hpp"
#include "defiVia.hpp"

BEGIN_DEF_PARSER_NAMESPACE

/* NEW CALLBACK - If you are creating a new .cpp and .hpp file to
 * describe a new class of object in the parser, then add a reference
 * to the .hpp here.
 *
 *  You must also add an entry for the .h and the .hpp in the package_list
 * file of the ../../../release directory. */

END_DEF_PARSER_NAMESPACE

#endif
