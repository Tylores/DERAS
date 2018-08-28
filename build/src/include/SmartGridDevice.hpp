/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 *    Copyright (c) V2 Systems, LLC.  All rights reserved.
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
**********************************************************/

#ifndef SMARTGRIDDEVICE_HPP_INCLUDED
#define SMARTGRIDDEVICE_HPP_INCLUDED

#include "DistributedEnergyResource.hpp"

class SmartGridDevice : public ajn::BusObject {
public:
    // member methods
    SmartGridDevice (ajn::BusAttachment* bus, 
                     const char* name, 
                     const char* path);

private:
    // member properties
    ajn::BusAttachment* bus_;
    const char* interface_;
    const char* name_;
    const ajn::InterfaceDescription::Member* signal_;

};

#endif // SMARTGRIDDEVICE_HPP_INCLUDED
