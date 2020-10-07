package lmi

import (
    "android/soong/android"
)

func init() {
    android.RegisterModuleType("lmi_fod_hal_binary", fodHalBinaryFactory)
}
