/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <unistd.h>
#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using android::init::property_set;

std::vector<std::string> ro_props_default_source_order =
{
    "",
    "bootimage.",
    "odm.",
    "product.",
    "system.",
    "vendor.",
};

void property_override(char const prop[], char const value[], bool add = true)
{
    prop_info *pi;

    pi = (prop_info *) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void set_ro_build_prop(const std::string &prop, const std::string &value)
{
    for (const auto &source : ro_props_default_source_order) {
        auto prop_name = "ro." + source + "build." + prop;
        if (source == "")
            property_override(prop_name.c_str(), value.c_str());
        else
            property_override(prop_name.c_str(), value.c_str(), false);
    }
};

void set_ro_product_prop(const std::string &prop, const std::string &value)
{
    for (const auto &source : ro_props_default_source_order) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    }
};

void vendor_load_properties() {
    std::string region;
    region = GetProperty("ro.boot.hwc", "GLOBAL");

    std::string model;
    std::string device;
    std::string fingerprint;
    std::string description;
    std::string mod_device;

    if (region == "GLOBAL")
    {
        model = "Poco F2 Pro";
        device = "lmi";
        fingerprint = "Redmi/lmi/lmi:10/QKQ1.191117.002/V12.0.4.0.QJKMIXM:user/release-keys";
        description = "lmi-user 10 QKQ1.191117.002 V12.0.4.0.QJKMIXM release-keys";
        mod_device = "lmi_global";
    }
    else if (region == "CN")
    {
        model = "Redmi K30 Pro";
        device = "lmi";
        fingerprint = "Redmi/lmi/lmi:10/QKQ1.191117.002/V12.0.4.0.QJKMIXM:user/release-keys";
        description = "lmi-user 10 QKQ1.191117.002 V12.0.4.0.QJKMIXM release-keys";
        mod_device = "lmipro";
    }

    // SafetyNet workaround
    property_override("ro.boot.verifiedbootstate", "green");
    fingerprint = "google/sunfish/sunfish:11/RP1A.201005.006/6828489:user/release-keys";

    set_ro_build_prop("fingerprint", fingerprint);
    set_ro_product_prop("device", device);
    set_ro_product_prop("model", model);
    property_override("ro.build.description", description.c_str());
    if (mod_device != "") {
        property_override("ro.product.mod_device", mod_device.c_str());
    }
}
