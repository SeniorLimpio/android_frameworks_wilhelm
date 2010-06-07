/*
 * Copyright (C) 2010 The Android Open Source Project
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

/* PrefetchStatus implementation */

#include "sles_allinclusive.h"

static SLresult IPrefetchStatus_GetPrefetchStatus(SLPrefetchStatusItf self, SLuint32 *pStatus)
{
    if (NULL == pStatus)
        return SL_RESULT_PARAMETER_INVALID;
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    interface_lock_peek(this);
    SLuint32 status = this->mStatus;
    interface_unlock_peek(this);
    *pStatus = status;
    return SL_RESULT_SUCCESS;
}

static SLresult IPrefetchStatus_GetFillLevel(SLPrefetchStatusItf self, SLpermille *pLevel)
{
    if (NULL == pLevel)
        return SL_RESULT_PARAMETER_INVALID;
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    interface_lock_peek(this);
    SLpermille level = this->mLevel;
    interface_unlock_peek(this);
    *pLevel = level;
    return SL_RESULT_SUCCESS;
}

static SLresult IPrefetchStatus_RegisterCallback(SLPrefetchStatusItf self,
    slPrefetchCallback callback, void *pContext)
{
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    interface_lock_exclusive(this);
    this->mCallback = callback;
    this->mContext = pContext;
    interface_unlock_exclusive(this);
    return SL_RESULT_SUCCESS;
}

static SLresult IPrefetchStatus_SetCallbackEventsMask(SLPrefetchStatusItf self, SLuint32 eventFlags)
{
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    interface_lock_poke(this);
    this->mCallbackEventsMask = eventFlags;
    interface_unlock_poke(this);
    return SL_RESULT_SUCCESS;
}

static SLresult IPrefetchStatus_GetCallbackEventsMask(SLPrefetchStatusItf self,
    SLuint32 *pEventFlags)
{
    if (NULL == pEventFlags)
        return SL_RESULT_PARAMETER_INVALID;
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    interface_lock_peek(this);
    SLuint32 callbackEventsMask = this->mCallbackEventsMask;
    interface_unlock_peek(this);
    *pEventFlags = callbackEventsMask;
    return SL_RESULT_SUCCESS;
}

static SLresult IPrefetchStatus_SetFillUpdatePeriod(SLPrefetchStatusItf self, SLpermille period)
{
    if (0 == period)
        return SL_RESULT_PARAMETER_INVALID;
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    interface_lock_poke(this);
    this->mFillUpdatePeriod = period;
    interface_unlock_poke(this);
    return SL_RESULT_SUCCESS;
}

static SLresult IPrefetchStatus_GetFillUpdatePeriod(SLPrefetchStatusItf self, SLpermille *pPeriod)
{
    if (NULL == pPeriod)
        return SL_RESULT_PARAMETER_INVALID;
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    interface_lock_peek(this);
    SLpermille fillUpdatePeriod = this->mFillUpdatePeriod;
    interface_unlock_peek(this);
    *pPeriod = fillUpdatePeriod;
    return SL_RESULT_SUCCESS;
}

static const struct SLPrefetchStatusItf_ IPrefetchStatus_Itf = {
    IPrefetchStatus_GetPrefetchStatus,
    IPrefetchStatus_GetFillLevel,
    IPrefetchStatus_RegisterCallback,
    IPrefetchStatus_SetCallbackEventsMask,
    IPrefetchStatus_GetCallbackEventsMask,
    IPrefetchStatus_SetFillUpdatePeriod,
    IPrefetchStatus_GetFillUpdatePeriod
};

void IPrefetchStatus_init(void *self)
{
    IPrefetchStatus *this = (IPrefetchStatus *) self;
    this->mItf = &IPrefetchStatus_Itf;
    this->mStatus = SL_PREFETCHSTATUS_UNDERFLOW;
    this->mLevel = 0;
    this->mCallback = NULL;
    this->mContext = NULL;
    this->mCallbackEventsMask = 0;
    this->mFillUpdatePeriod = 100;
}
