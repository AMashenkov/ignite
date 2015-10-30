/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ignite/impl/interop/interop.h"
#include "ignite/impl/portable/portable_common.h"
#include "ignite/impl/portable/portable_id_resolver.h"
#include "ignite/impl/portable/portable_reader_impl.h"
#include "ignite/impl/portable/portable_utils.h"
#include "ignite/portable/portable_type.h"
#include "ignite/ignite_error.h"
#include "ignite/impl/interop/interop_stream_position_guard.h"

using namespace ignite::impl::interop;
using namespace ignite::impl::portable;
using namespace ignite::portable;

namespace ignite
{
    namespace impl
    {
        namespace portable
        {
            PortableReaderImpl::PortableReaderImpl(InteropInputStream* stream, PortableIdResolver* idRslvr,
                int32_t pos, bool usrType, int32_t typeId, int32_t hashCode, int32_t len, int32_t rawOff,
                int32_t footerBegin, int32_t footerEnd) :
                stream(stream), idRslvr(idRslvr), pos(pos), usrType(usrType), typeId(typeId), 
                hashCode(hashCode), len(len), rawOff(rawOff), rawMode(false), elemIdGen(0), elemId(0),
                elemCnt(-1), elemRead(0), footerBegin(footerBegin), footerEnd(footerEnd)
            {
                // No-op.
            }

            PortableReaderImpl::PortableReaderImpl(InteropInputStream* stream) :
                stream(stream), idRslvr(NULL), pos(0), usrType(false), typeId(0), hashCode(0), len(0),
                rawOff(0), rawMode(true), elemIdGen(0), elemId(0), elemCnt(-1), elemRead(0), footerBegin(-1),
                footerEnd(-1)
            {
                // No-op.
            }

            int8_t PortableReaderImpl::ReadInt8()
            {
                return ReadRaw<int8_t>(PortableUtils::ReadInt8);                
            }
            
            int32_t PortableReaderImpl::ReadInt8Array(int8_t* res, const int32_t len)
            {
                return ReadRawArray<int8_t>(res, len, PortableUtils::ReadInt8Array, IGNITE_TYPE_ARRAY_BYTE);
            }

            int8_t PortableReaderImpl::ReadInt8(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadInt8, IGNITE_TYPE_BYTE, static_cast<int8_t>(0));
            }

            int32_t PortableReaderImpl::ReadInt8Array(const char* fieldName, int8_t* res, const int32_t len)
            {
                return ReadArray<int8_t>(fieldName, res, len,PortableUtils::ReadInt8Array, IGNITE_TYPE_ARRAY_BYTE);
            }

            bool PortableReaderImpl::ReadBool()
            {
                return ReadRaw<bool>(PortableUtils::ReadBool);
            }

            int32_t PortableReaderImpl::ReadBoolArray(bool* res, const int32_t len)
            {
                return ReadRawArray<bool>(res, len, PortableUtils::ReadBoolArray, IGNITE_TYPE_ARRAY_BOOL);
            }

            bool PortableReaderImpl::ReadBool(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadBool, IGNITE_TYPE_BOOL, static_cast<bool>(0));
            }

            int32_t PortableReaderImpl::ReadBoolArray(const char* fieldName, bool* res, const int32_t len)
            {
                return ReadArray<bool>(fieldName, res, len,PortableUtils::ReadBoolArray, IGNITE_TYPE_ARRAY_BOOL);
            }

            int16_t PortableReaderImpl::ReadInt16()
            {
                return ReadRaw<int16_t>(PortableUtils::ReadInt16);
            }

            int32_t PortableReaderImpl::ReadInt16Array(int16_t* res, const int32_t len)
            {
                return ReadRawArray<int16_t>(res, len, PortableUtils::ReadInt16Array, IGNITE_TYPE_ARRAY_SHORT);
            }

            int16_t PortableReaderImpl::ReadInt16(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadInt16, IGNITE_TYPE_SHORT, static_cast<int16_t>(0));
            }

            int32_t PortableReaderImpl::ReadInt16Array(const char* fieldName, int16_t* res, const int32_t len)
            {
                return ReadArray<int16_t>(fieldName, res, len, PortableUtils::ReadInt16Array, IGNITE_TYPE_ARRAY_SHORT);
            }

            uint16_t PortableReaderImpl::ReadUInt16()
            {
                return ReadRaw<uint16_t>(PortableUtils::ReadUInt16);
            }

            int32_t PortableReaderImpl::ReadUInt16Array(uint16_t* res, const int32_t len)
            {
                return ReadRawArray<uint16_t>(res, len, PortableUtils::ReadUInt16Array, IGNITE_TYPE_ARRAY_CHAR);
            }

            uint16_t PortableReaderImpl::ReadUInt16(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadUInt16, IGNITE_TYPE_CHAR, static_cast<uint16_t>(0));
            }

            int32_t PortableReaderImpl::ReadUInt16Array(const char* fieldName, uint16_t* res, const int32_t len)
            {
                return ReadArray<uint16_t>(fieldName, res, len,PortableUtils::ReadUInt16Array, IGNITE_TYPE_ARRAY_CHAR);
            }

            int32_t PortableReaderImpl::ReadInt32()
            {
                return ReadRaw<int32_t>(PortableUtils::ReadInt32);
            }

            int32_t PortableReaderImpl::ReadInt32Array(int32_t* res, const int32_t len)
            {
                return ReadRawArray<int32_t>(res, len, PortableUtils::ReadInt32Array, IGNITE_TYPE_ARRAY_INT);
            }

            int32_t PortableReaderImpl::ReadInt32(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadInt32, IGNITE_TYPE_INT, static_cast<int32_t>(0));
            }

            int32_t PortableReaderImpl::ReadInt32Array(const char* fieldName, int32_t* res, const int32_t len)
            {
                return ReadArray<int32_t>(fieldName, res, len,PortableUtils::ReadInt32Array, IGNITE_TYPE_ARRAY_INT);
            }

            int64_t PortableReaderImpl::ReadInt64()
            {
                return ReadRaw<int64_t>(PortableUtils::ReadInt64);
            }

            int32_t PortableReaderImpl::ReadInt64Array(int64_t* res, const int32_t len)
            {
                return ReadRawArray<int64_t>(res, len, PortableUtils::ReadInt64Array, IGNITE_TYPE_ARRAY_LONG);
            }

            int64_t PortableReaderImpl::ReadInt64(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadInt64, IGNITE_TYPE_LONG, static_cast<int64_t>(0));
            }

            int32_t PortableReaderImpl::ReadInt64Array(const char* fieldName, int64_t* res, const int32_t len)
            {
                return ReadArray<int64_t>(fieldName, res, len,PortableUtils::ReadInt64Array, IGNITE_TYPE_ARRAY_LONG);
            }

            float PortableReaderImpl::ReadFloat()
            {
                return ReadRaw<float>(PortableUtils::ReadFloat);
            }

            int32_t PortableReaderImpl::ReadFloatArray(float* res, const int32_t len)
            {
                return ReadRawArray<float>(res, len, PortableUtils::ReadFloatArray, IGNITE_TYPE_ARRAY_FLOAT);
            }

            float PortableReaderImpl::ReadFloat(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadFloat, IGNITE_TYPE_FLOAT, static_cast<float>(0));
            }

            int32_t PortableReaderImpl::ReadFloatArray(const char* fieldName, float* res, const int32_t len)
            {
                return ReadArray<float>(fieldName, res, len,PortableUtils::ReadFloatArray, IGNITE_TYPE_ARRAY_FLOAT);
            }

            double PortableReaderImpl::ReadDouble()
            {
                return ReadRaw<double>(PortableUtils::ReadDouble);
            }

            int32_t PortableReaderImpl::ReadDoubleArray(double* res, const int32_t len)
            {
                return ReadRawArray<double>(res, len, PortableUtils::ReadDoubleArray, IGNITE_TYPE_ARRAY_DOUBLE);
            }

            double PortableReaderImpl::ReadDouble(const char* fieldName)
            {
                return Read(fieldName, PortableUtils::ReadDouble, IGNITE_TYPE_DOUBLE, static_cast<double>(0));
            }

            int32_t PortableReaderImpl::ReadDoubleArray(const char* fieldName, double* res, const int32_t len)
            {
                return ReadArray<double>(fieldName, res, len,PortableUtils::ReadDoubleArray, IGNITE_TYPE_ARRAY_DOUBLE);
            }

            Guid PortableReaderImpl::ReadGuid()
            {
                CheckRawMode(true);
                CheckSingleMode(true);

                return ReadNullable(stream, PortableUtils::ReadGuid, IGNITE_TYPE_UUID);
            }

            int32_t PortableReaderImpl::ReadGuidArray(Guid* res, const int32_t len)
            {
                CheckRawMode(true);
                CheckSingleMode(true);

                return ReadArrayInternal<Guid>(res, len, stream, ReadGuidArrayInternal, IGNITE_TYPE_ARRAY_UUID);
            }

            Guid PortableReaderImpl::ReadGuid(const char* fieldName)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                    return Guid();

                stream->Position(fieldPos);

                return ReadNullable(stream, PortableUtils::ReadGuid, IGNITE_TYPE_UUID);
            }

            int32_t PortableReaderImpl::ReadGuidArray(const char* fieldName, Guid* res, const int32_t len)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                int32_t pos = stream->Position();

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                    return -1;

                stream->Position(fieldPos);

                int32_t realLen = ReadArrayInternal<Guid>(res, len, stream, ReadGuidArrayInternal, IGNITE_TYPE_ARRAY_UUID);

                return realLen;
            }

            void PortableReaderImpl::ParseHeaderIfNeeded()
            {
                if (footerBegin)
                    return;

                InteropStreamPositionGuard<InteropInputStream> posGuard(*stream);

                int8_t hdr = stream->ReadInt8();

                if (hdr != IGNITE_HDR_FULL)
                    IGNITE_ERROR_2(ignite::IgniteError::IGNITE_ERR_PORTABLE, "Invalid header: ", hdr);

                int8_t protoVer = stream->ReadInt8();

                if (protoVer != IGNITE_PROTO_VER) {
                    IGNITE_ERROR_2(ignite::IgniteError::IGNITE_ERR_PORTABLE,
                        "Unsupported portable protocol version: ", protoVer);
                }

                int16_t flags = stream->ReadInt16();
                int32_t typeId = stream->ReadInt32();
                int32_t hashCode = stream->ReadInt32();
                int32_t len = stream->ReadInt32();
                int32_t schemaId = stream->ReadInt32();
                int32_t schemaOrRawOff = stream->ReadInt32();

                if (flags & IGNITE_PORTABLE_FLAG_RAW_ONLY)
                {
                    footerBegin = len;

                    rawOff = schemaOrRawOff;
                }
                else
                {
                    footerBegin = schemaOrRawOff;

                    rawOff = (len - footerBegin) % 8 ? stream->ReadInt32(pos + len - 4) : schemaOrRawOff;
                }

                footerEnd = len - ((len - footerBegin) % 8);

                bool usrType = flags & IGNITE_PORTABLE_FLAG_USER_OBJECT;
            }

            void PortableReaderImpl::ReadGuidArrayInternal(InteropInputStream* stream, Guid* res, const int32_t len)
            {
                for (int i = 0; i < len; i++)
                    *(res + i) = ReadNullable<Guid>(stream, PortableUtils::ReadGuid, IGNITE_TYPE_UUID);
            }

            int32_t PortableReaderImpl::ReadString(char* res, const int32_t len)
            {
                CheckRawMode(true);
                CheckSingleMode(true);

                return ReadStringInternal(res, len);
            }

            int32_t PortableReaderImpl::ReadString(const char* fieldName, char* res, const int32_t len)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                int32_t pos = stream->Position();
                
                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                    return -1;

                stream->Position(fieldPos);

                int32_t realLen = ReadStringInternal(res, len);

                return realLen;
            }

            int32_t PortableReaderImpl::ReadStringArray(int32_t* size)
            {
                return StartContainerSession(true, IGNITE_TYPE_ARRAY_STRING, size);
            }

            int32_t PortableReaderImpl::ReadStringArray(const char* fieldName, int32_t* size)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                {
                    *size = -1;

                    return ++elemIdGen;
                }

                stream->Position(fieldPos);

                return StartContainerSession(false, IGNITE_TYPE_ARRAY_STRING, size);
            }

            int32_t PortableReaderImpl::ReadStringElement(int32_t id, char* res, const int32_t len)
            {
                CheckSession(id);

                int32_t posBefore = stream->Position();

                int32_t realLen = ReadStringInternal(res, len);

                int32_t posAfter = stream->Position();

                if (posAfter > posBefore && ++elemRead == elemCnt) {
                    elemId = 0;
                    elemCnt = -1;
                    elemRead = 0;
                }

                return realLen;
            }

            int32_t PortableReaderImpl::ReadStringInternal(char* res, const int32_t len)
            {
                int8_t hdr = stream->ReadInt8();

                if (hdr == IGNITE_TYPE_STRING) {
                    bool utf8Mode = stream->ReadBool();
                    int32_t realLen = stream->ReadInt32();

                    if (res && len >= realLen) {
                        if (utf8Mode)
                        {
                            for (int i = 0; i < realLen; i++)
                                *(res + i) = static_cast<char>(stream->ReadInt8());
                        }
                        else
                        {
                            for (int i = 0; i < realLen; i++)
                                *(res + i) = static_cast<char>(stream->ReadUInt16());
                        }

                        if (len > realLen)
                            *(res + realLen) = 0; // Set NULL terminator if possible.
                    }
                    else
                        stream->Position(stream->Position() - 6);

                    return realLen;
                }
                else if (hdr != IGNITE_HDR_NULL)
                    ThrowOnInvalidHeader(IGNITE_TYPE_ARRAY, hdr);

                return -1;
            }

            int32_t PortableReaderImpl::ReadArray(int32_t* size)
            {
                return StartContainerSession(true, IGNITE_TYPE_ARRAY, size);
            }

            int32_t PortableReaderImpl::ReadArray(const char* fieldName, int32_t* size)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                {
                    *size = -1;

                    return ++elemIdGen;
                }

                stream->Position(fieldPos);

                return StartContainerSession(false, IGNITE_TYPE_ARRAY, size);
            }

            int32_t PortableReaderImpl::ReadCollection(CollectionType* typ, int32_t* size)
            {
                int32_t id = StartContainerSession(true, IGNITE_TYPE_COLLECTION, size);

                if (*size == -1)
                    *typ = IGNITE_COLLECTION_UNDEFINED;
                else
                    *typ = static_cast<CollectionType>(stream->ReadInt8());

                return id;
            }

            int32_t PortableReaderImpl::ReadCollection(const char* fieldName, CollectionType* typ, int32_t* size)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                {
                    *typ = IGNITE_COLLECTION_UNDEFINED;
                    *size = -1;

                    return ++elemIdGen;
                }

                stream->Position(fieldPos);

                int32_t id = StartContainerSession(false, IGNITE_TYPE_COLLECTION, size);

                if (*size == -1)
                    *typ = IGNITE_COLLECTION_UNDEFINED;
                else
                    *typ = static_cast<CollectionType>(stream->ReadInt8());

                return id;
            }

            int32_t PortableReaderImpl::ReadMap(MapType* typ, int32_t* size)
            {
                int32_t id = StartContainerSession(true, IGNITE_TYPE_MAP, size);

                if (*size == -1)
                    *typ = IGNITE_MAP_UNDEFINED;
                else
                    *typ = static_cast<MapType>(stream->ReadInt8());

                return id;
            }

            int32_t PortableReaderImpl::ReadMap(const char* fieldName, MapType* typ, int32_t* size)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                {
                    *typ = IGNITE_MAP_UNDEFINED;
                    *size = -1;

                    return ++elemIdGen;
                }

                stream->Position(fieldPos);

                int32_t id = StartContainerSession(false, IGNITE_TYPE_MAP, size);

                if (*size == -1)
                    *typ = IGNITE_MAP_UNDEFINED;
                else
                    *typ = static_cast<MapType>(stream->ReadInt8());

                return id;
            }

            CollectionType PortableReaderImpl::ReadCollectionTypeUnprotected()
            {
                int32_t size = ReadCollectionSizeUnprotected();
                if (size == -1)
                    return IGNITE_COLLECTION_UNDEFINED;

                CollectionType typ = static_cast<CollectionType>(stream->ReadInt8());

                return typ;
            }

            CollectionType PortableReaderImpl::ReadCollectionType()
            {
                InteropStreamPositionGuard<InteropInputStream> positionGuard(*stream);
                
                return ReadCollectionTypeUnprotected();
            }

            CollectionType PortableReaderImpl::ReadCollectionType(const char* fieldName)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                InteropStreamPositionGuard<InteropInputStream> positionGuard(*stream);

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                    return IGNITE_COLLECTION_UNDEFINED;

                stream->Position(fieldPos);

                return ReadCollectionTypeUnprotected();
            }

            int32_t PortableReaderImpl::ReadCollectionSizeUnprotected()
            {
                int8_t hdr = stream->ReadInt8();

                if (hdr != IGNITE_TYPE_COLLECTION)
                {
                    if (hdr != IGNITE_HDR_NULL)
                        ThrowOnInvalidHeader(IGNITE_TYPE_COLLECTION, hdr);

                    return -1;
                }

                int32_t size = stream->ReadInt32();

                return size;
            }

            int32_t PortableReaderImpl::ReadCollectionSize()
            {
                InteropStreamPositionGuard<InteropInputStream> positionGuard(*stream);

                return ReadCollectionSizeUnprotected();
            }

            int32_t PortableReaderImpl::ReadCollectionSize(const char* fieldName)
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                InteropStreamPositionGuard<InteropInputStream> positionGuard(*stream);

                int32_t fieldId = idRslvr->GetFieldId(typeId, fieldName);
                int32_t fieldPos = FindField(fieldId);

                if (fieldPos <= 0)
                    return -1;

                stream->Position(fieldPos);

                return ReadCollectionSizeUnprotected();
            }

            bool PortableReaderImpl::HasNextElement(int32_t id) const
            {
                return elemId == id && elemRead < elemCnt;
            }

            void PortableReaderImpl::SetRawMode()
            {
                CheckRawMode(false);
                CheckSingleMode(true);

                stream->Position(pos + rawOff);
                rawMode = true;
            }

            template <>
            int8_t PortableReaderImpl::ReadTopObject<int8_t>()
            {
                return ReadTopObject0(IGNITE_TYPE_BYTE, PortableUtils::ReadInt8, static_cast<int8_t>(0));
            }

            template <>
            bool PortableReaderImpl::ReadTopObject<bool>()
            {
                return ReadTopObject0(IGNITE_TYPE_BOOL, PortableUtils::ReadBool, static_cast<bool>(0));
            }

            template <>
            int16_t PortableReaderImpl::ReadTopObject<int16_t>()
            {
                return ReadTopObject0(IGNITE_TYPE_SHORT, PortableUtils::ReadInt16, static_cast<int16_t>(0));
            }

            template <>
            uint16_t PortableReaderImpl::ReadTopObject<uint16_t>()
            {
                return ReadTopObject0(IGNITE_TYPE_CHAR, PortableUtils::ReadUInt16, static_cast<uint16_t>(0));
            }

            template <>
            int32_t PortableReaderImpl::ReadTopObject<int32_t>()
            {
                return ReadTopObject0(IGNITE_TYPE_INT, PortableUtils::ReadInt32, static_cast<int32_t>(0));
            }

            template <>
            int64_t PortableReaderImpl::ReadTopObject<int64_t>()
            {
                return ReadTopObject0(IGNITE_TYPE_LONG, PortableUtils::ReadInt64, static_cast<int64_t>(0));
            }

            template <>
            float PortableReaderImpl::ReadTopObject<float>()
            {
                return ReadTopObject0(IGNITE_TYPE_FLOAT, PortableUtils::ReadFloat, static_cast<float>(0));
            }

            template <>
            double PortableReaderImpl::ReadTopObject<double>()
            {
                return ReadTopObject0(IGNITE_TYPE_DOUBLE, PortableUtils::ReadDouble, static_cast<double>(0));
            }

            template <>
            Guid PortableReaderImpl::ReadTopObject<Guid>()
            {
                int8_t typeId = stream->ReadInt8();

                if (typeId == IGNITE_TYPE_UUID)
                    return PortableUtils::ReadGuid(stream);
                else if (typeId == IGNITE_HDR_NULL)
                    return Guid();
                else {
                    int32_t pos = stream->Position() - 1;

                    IGNITE_ERROR_FORMATTED_3(IgniteError::IGNITE_ERR_PORTABLE, "Invalid header", "position", pos, "expected", IGNITE_TYPE_UUID, "actual", typeId)
                }
            }

            InteropInputStream* PortableReaderImpl::GetStream()
            {
                return stream;
            }

            int32_t PortableReaderImpl::FindField(const int32_t fieldId)
            {
                InteropStreamPositionGuard<InteropInputStream> streamGuard(*stream);

                stream->Position(footerBegin);

                for (int32_t schemaPos = footerBegin; schemaPos < footerEnd; schemaPos += 8)
                {
                    int32_t currentFieldId = stream->ReadInt32(schemaPos);

                    if (fieldId == currentFieldId)
                        return stream->ReadInt32(schemaPos + 4) + pos;
                }

                return -1;
            }

            void PortableReaderImpl::CheckRawMode(bool expected) const
            {
                if (expected && !rawMode) {
                    IGNITE_ERROR_1(IgniteError::IGNITE_ERR_PORTABLE, "Operation can be performed only in raw mode.")
                }
                else if (!expected && rawMode) {
                    IGNITE_ERROR_1(IgniteError::IGNITE_ERR_PORTABLE, "Operation cannot be performed in raw mode.")
                }
            }

            void PortableReaderImpl::CheckSingleMode(bool expected) const
            {
                if (expected && elemId != 0) {
                    IGNITE_ERROR_1(IgniteError::IGNITE_ERR_PORTABLE, "Operation cannot be performed when container is being read.");
                }
                else if (!expected && elemId == 0) {
                    IGNITE_ERROR_1(IgniteError::IGNITE_ERR_PORTABLE, "Operation can be performed only when container is being read.");
                }
            }

            int32_t PortableReaderImpl::StartContainerSession(bool expRawMode, int8_t expHdr, int32_t* size)
            {
                CheckRawMode(expRawMode);
                CheckSingleMode(true);

                int8_t hdr = stream->ReadInt8();

                if (hdr == expHdr)
                {
                    int32_t cnt = stream->ReadInt32();

                    if (cnt != 0) 
                    {
                        elemId = ++elemIdGen;
                        elemCnt = cnt;
                        elemRead = 0;

                        *size = cnt;

                        return elemId;
                    }
                    else
                    {
                        *size = 0;

                        return ++elemIdGen;
                    }
                }
                else if (hdr == IGNITE_HDR_NULL) {
                    *size = -1;

                    return ++elemIdGen;
                }
                else {
                    ThrowOnInvalidHeader(expHdr, hdr);

                    return 0;
                }
            }

            void PortableReaderImpl::CheckSession(int32_t expSes) const
            {
                if (elemId != expSes) {
                    IGNITE_ERROR_1(IgniteError::IGNITE_ERR_PORTABLE, "Containter read session has been finished or is not started yet.");
                }
            }

            void PortableReaderImpl::ThrowOnInvalidHeader(int32_t pos, int8_t expHdr, int8_t hdr)
            {
                IGNITE_ERROR_FORMATTED_3(IgniteError::IGNITE_ERR_PORTABLE, "Invalid header", "position", pos, "expected", expHdr, "actual", hdr)
            }

            void PortableReaderImpl::ThrowOnInvalidHeader(int8_t expHdr, int8_t hdr) const
            {
                int32_t pos = stream->Position() - 1;

                ThrowOnInvalidHeader(pos, expHdr, hdr);
            }
        }
    }
}