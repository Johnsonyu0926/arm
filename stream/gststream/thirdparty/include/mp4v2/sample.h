// Score: 100

#ifndef MP4V2_SAMPLE_H
#define MP4V2_SAMPLE_H

typedef enum MP4SampleDependencyType_e {
    MP4_SDT_UNKNOWN                       = 0x00, /**< unknown */
    MP4_SDT_HAS_REDUNDANT_CODING          = 0x01, /**< contains redundant coding */
    MP4_SDT_HAS_NO_REDUNDANT_CODING       = 0x02, /**< does not contain redundant coding */
    MP4_SDT_HAS_DEPENDENTS                = 0x04, /**< referenced by other samples */
    MP4_SDT_HAS_NO_DEPENDENTS             = 0x08, /**< not referenced by other samples */
    MP4_SDT_IS_DEPENDENT                  = 0x10, /**< references other samples */
    MP4_SDT_IS_INDEPENDENT                = 0x20, /**< does not reference other samples */
    MP4_SDT_EARLIER_DISPLAY_TIMES_ALLOWED = 0x40, /**< subequent samples in GOP may display earlier */
    _MP4_SDT_RESERVED                     = 0x80 /**< reserved */
} MP4SampleDependencyType;

MP4V2_EXPORT
bool MP4ReadSample(
    /* input parameters */
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId,
    /* input/output parameters */
    uint8_t** ppBytes,
    uint32_t* pNumBytes,
    /* output parameters */
    MP4Timestamp* pStartTime DEFAULT(NULL),
    MP4Duration*  pDuration DEFAULT(NULL),
    MP4Duration*  pRenderingOffset DEFAULT(NULL),
    bool*         pIsSyncSample DEFAULT(NULL) );

MP4V2_EXPORT
bool MP4ReadSampleFromTime(
    /* input parameters */
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Timestamp  when,
    /* input/output parameters */
    uint8_t** ppBytes,
    uint32_t* pNumBytes,
    /* output parameters */
    MP4Timestamp* pStartTime DEFAULT(NULL),
    MP4Duration*  pDuration DEFAULT(NULL),
    MP4Duration*  pRenderingOffset DEFAULT(NULL),
    bool*         pIsSyncSample DEFAULT(NULL) );

MP4V2_EXPORT
bool MP4WriteSample(
    MP4FileHandle  hFile,
    MP4TrackId     trackId,
    const uint8_t* pBytes,
    uint32_t       numBytes,
    MP4Duration    duration DEFAULT(MP4_INVALID_DURATION),
    MP4Duration    renderingOffset DEFAULT(0),
    bool           isSyncSample DEFAULT(true) );

MP4V2_EXPORT
bool MP4WriteSampleDependency(
    MP4FileHandle  hFile,
    MP4TrackId     trackId,
    const uint8_t* pBytes,
    uint32_t       numBytes,
    MP4Duration    duration,
    MP4Duration    renderingOffset,
    bool           isSyncSample,
    uint32_t       dependencyFlags );

MP4V2_EXPORT
bool MP4CopySample(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4SampleId   srcSampleId,
    MP4FileHandle dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    MP4TrackId    dstTrackId DEFAULT(MP4_INVALID_TRACK_ID),
    MP4Duration   dstSampleDuration DEFAULT(MP4_INVALID_DURATION) );

MP4V2_EXPORT
bool MP4EncAndCopySample(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4SampleId   srcSampleId,
    encryptFunc_t encfcnp,
    uint32_t      encfcnparam1,
    MP4FileHandle dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    MP4TrackId    dstTrackId DEFAULT(MP4_INVALID_TRACK_ID),
    MP4Duration   dstSampleDuration DEFAULT(MP4_INVALID_DURATION) );

/** Not implemented.
 */
MP4V2_EXPORT
bool MP4ReferenceSample(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4SampleId   srcSampleId,
    MP4FileHandle dstFile,
    MP4TrackId    dstTrackId,
    MP4Duration   dstSampleDuration DEFAULT(MP4_INVALID_DURATION) );

MP4V2_EXPORT
uint32_t MP4GetSampleSize(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId);

MP4V2_EXPORT
uint32_t MP4GetTrackMaxSampleSize(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
MP4SampleId MP4GetSampleIdFromTime(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Timestamp  when,
    bool          wantSyncSample DEFAULT(false) );

MP4V2_EXPORT
MP4Timestamp MP4GetSampleTime(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

MP4V2_EXPORT
MP4Duration MP4GetSampleDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

MP4V2_EXPORT
MP4Duration MP4GetSampleRenderingOffset(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

MP4V2_EXPORT
bool MP4SetSampleRenderingOffset(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId,
    MP4Duration   renderingOffset );

MP4V2_EXPORT
int8_t MP4GetSampleSync(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

/* @} ***********************************************************************/

#endif /* MP4V2_SAMPLE_H */

// By GST @Date