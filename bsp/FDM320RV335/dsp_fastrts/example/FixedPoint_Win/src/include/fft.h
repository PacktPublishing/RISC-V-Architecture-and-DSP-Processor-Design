/*
 * fft.h
 *
 *  Created on: 2024Äê1ÔÂ24ÈÕ
 *      Author:
 */

#ifndef _FFT_H_
#define _FFT_H_

//*****************************************************************************
// the includes
//*****************************************************************************
#include <stdint.h>

//!
//! \defgroup FFT Complex and Real FFT

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//defines
//*****************************************************************************
#define NULL    0

//! \brief Initialization defaults for the 8-point complex FFT
//!
#define CFFT32_8P_DEFAULTS    {     \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    8,                              \
    3,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    128,                            \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}

//! \brief Initialization defaults for the 16-point complex FFT
//!
#define CFFT32_16P_DEFAULTS    {    \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    16,                             \
    4,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    256,                            \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}
//   ratio 256=4096/16

//! \brief Initialization defaults for the 32-point complex FFT
//!
#define CFFT32_32P_DEFAULTS    {    \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    32,                             \
    5,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    128,                            \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}
//   ratio 128=4096/32

//! \brief Initialization defaults for the 64-point complex FFT
//!
#define CFFT32_64P_DEFAULTS    {    \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    64,                             \
    6,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    64,                             \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}
//   ratio 64=4096/64

//! \brief Initialization defaults for the 128-point complex FFT
//!
#define CFFT32_128P_DEFAULTS    {   \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    128,                            \
    7,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    32,                             \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}

//! \brief Initialization defaults for the 256-point complex FFT
//!
#define CFFT32_256P_DEFAULTS    {   \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    256,                            \
    8,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    16,                             \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}

//! \brief Initialization defaults for the 512-point complex FFT
//!
#define CFFT32_512P_DEFAULTS    {   \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    512,                            \
    9,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    8,                              \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}

//! \brief Initialization defaults for the 1024-point complex FFT
//!
#define CFFT32_1024P_DEFAULTS    {  \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    1024,                           \
    10,                             \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    4,                              \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}

//! \brief Initialization defaults for the 2048-point complex FFT
//!
#define CFFT32_2048P_DEFAULTS    {  \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    2048,                           \
    11,                             \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    2,                              \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}

//! \brief Initialization defaults for the 4096-point complex FFT
//!
#define CFFT32_4096P_DEFAULTS    {  \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    4096,                           \
    12,                             \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    1,                              \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_izero,  \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))CFFT32_win}


//! \brief Initialization defaults for the 32-point real FFT
//!
#define RFFT32_32P_DEFAULTS    {    \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    16,                             \
    4,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    256,                            \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))RFFT32_split, \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))RFFT32_win}

//! \brief Initialization defaults for the 64-point real FFT
//!
#define RFFT32_64P_DEFAULTS    {    \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    32,                             \
    5,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    128,                             \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))RFFT32_split, \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))RFFT32_win}

//! \brief Initialization defaults for the 128-point real FFT
//!
#define RFFT32_128P_DEFAULTS    {   \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    64,                             \
    6,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    64,                             \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))RFFT32_split, \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))RFFT32_win}

//! \brief Initialization defaults for the 256-point real FFT
//!
#define RFFT32_256P_DEFAULTS    {   \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    128,                            \
    7,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    32,                             \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))RFFT32_split, \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))RFFT32_win}

//! \brief Initialization defaults for the 512-point real FFT
//!
#define RFFT32_512P_DEFAULTS    {   \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    256,                            \
    8,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    16,                              \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))RFFT32_split, \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))RFFT32_win}

//! \brief Initialization defaults for the 1024-point real FFT
//!
#define RFFT32_1024P_DEFAULTS    {  \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    512,                            \
    9,                              \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    8,                              \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))RFFT32_split, \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))RFFT32_win}

//! \brief Initialization defaults for the 2048-point real FFT
//!
#define RFFT32_2048P_DEFAULTS    {  \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    1024,                           \
    10,                             \
    (int32_t *)NULL,                \
    (int32_t *)NULL,                \
    0,                              \
    0,                              \
    4,                              \
    (void (*)(void *))FFT32_init,   \
    (void (*)(void *))FFT32_calc,   \
    (void (*)(void *))RFFT32_split, \
    (void (*)(void *))FFT32_mag,   \
    (void (*)(void *))RFFT32_win}
//! \brief Initialization defaults for the 4096-point real FFT
//!
#define RFFT32_4096P_DEFAULTS    { \
    (int32_t *)NULL,               \
    (int32_t *)NULL,               \
    2048,                          \
    11,                            \
    (int32_t *)NULL,               \
    (int32_t *)NULL,               \
    0,                             \
    0,                             \
    2,                             \
    (void (*)(void *))FFT32_init,  \
    (void (*)(void *))FFT32_calc,  \
    (void (*)(void *))RFFT32_split,\
    (void (*)(void *))FFT32_mag,  \
    (void (*)(void *))RFFT32_win}

//*****************************************************************************
// typedefs
//*****************************************************************************
//! \brief 32-bit Complex FFT Structure
//!
typedef struct {
    int32_t *ipcbptr;      //!< Pointer to the input computation buffer
    int32_t *tfptr;        //!< Pointer to the twiddle factor tables
    int16_t size;          //!< Size of the FFT (a power of 2, max 4096)
    int16_t nrstage;       //!< Number of FFT stages (=log2(size))
    int32_t *magptr;       //!< Pointer to the magnitude buffer
    int32_t *winptr;       //!< Pointer to the window coefficients
    int32_t peakmag;       //!< Peak spectral magnitude value
    int16_t peakfrq;       //!< Frequency bin of the peak magnitude value
    int16_t ratio;         //!< Twiddle factor table skip value
    void (*init)(void *);  //!< Pointer to the FFT initialization function
    void (*izero)(void *); //!< Pointer to the "zero imaginary" function
    void (*calc)(void *);  //!< Pointer to the FFT calculation function
    void (*mag)(void *);   //!< Pointer to the FFT magnitude function
    void (*win)(void *);   //!< Pointer to the FFT windowing function
}CFFT32;

//! \brief Handle to the CFFT32 object
//!
typedef struct CFFT32 *CFFT32_Handle;

//! \brief 32-bit Real FFT Structure
//!
typedef struct {
    int32_t *ipcbptr;      //!< Pointer to the input computation buffer
    int32_t *tfptr;        //!< Pointer to the twiddle factor tables
    int16_t size;          //!< Size of the FFT (a power of 2, max 4096)
    int16_t nrstage;       //!< Number of FFT stages (=log2(size))
    int32_t *magptr;       //!< Pointer to the magnitude buffer
    int32_t *winptr;       //!< Pointer to the window coefficients
    int32_t peakmag;       //!< Peak spectral magnitude value
    int16_t peakfrq;       //!< Frequency bin of the peak magnitude value
    int16_t ratio;         //!< Twiddle factor table skip value
    void (*init)(void *);  //!< Pointer to the FFT initialization function
    void (*calc)(void *);  //!< Pointer to the FFT calculation function
    void (*split)(void *); //!< Pointer to the split function
    void (*mag)(void *);   //!< Pointer to the FFT magnitude function
    void (*win)(void *);   //!< Pointer to the FFT windowing function
}RFFT32;

//! \brief Handle to the RFFT32 object
//!
typedef struct RFFT32 *RFFT32_Handle;


//*****************************************************************************
// the function prototypes
//*****************************************************************************


//! \brief Windowing function for the 32-bit real FFT
//! \param pointer (handle) to an RFFT32 object
//! This function applies the window to a 2N point real data buffer that
//! has already been reordered in the bit-reversed format
void RFFT32_win(void *);
void FFT32_izero(void *);

//! \brief Split function for the 32-bit real FFT
//! \param pointer (handle) to an RFFT32 object
//! This function performs split operation to obtain 65 spectral bins of
//! 128-point real valued sequence from the 64-point complex FFT output
void RFFT32_split(void *);

//! \brief Magnitude function of the Real FFT
//! \param pointer (handle) to a RFFT32 object
//! his function computes the magnitude square of the Real FFT output. Allows in-place
//! and off-place storage of the magnitude square results. It calculates the magnitude
//! from DC (0Hz) to the Nyquist frequency (fs/2)
void FFT32_mag(void *);

//! \brief Bit Reversal routine for the 32-bit complex FFT
//! \param src  , pointer to the source array or buffer
//! \param dst  , pointer to the destination array or buffer
//! \param size , size of the buffer (must be a power of 2)
//! This function bit reverses the N-point real sequence stored in
//! alternate memory locations. If src=dst, then in-place bit reversal is performed.
//! The destination array should be aligned to 2N int32_t words, where N is the
//! size of the FFT.
void FFT32_brev(int32_t *src, int32_t *dst, uint16_t size);

//! \brief Windowing function for the 32-bit complex FFT
//! \param pointer (handle) to an CFFT32 object
//! This function applies the window to only the real portion of the N point
//! complex buffer that has already been reordered in the bit-reversed format
void CFFT32_win(void *);
void CFFT32_win_dual(void *);

//! \brief Complex FFT calculation routine
//! \param pointer (handle) to either an RFFT32 or CFFT32 object
//! This function will do an N point complex FFT calculation. It accepts handles to
//! both the RFFT32 and CFFT32 objects - the reason being that the N-pt Real
//! FFT is done using an N/2 point complex FFT
void FFT32_calc(void *);

//! \brief Complex FFT initialization routine
//! \param pointer (handle) to either an RFFT32 or CFFT32 object
//! This function sets the twiddle factor pointer "tfptr" element of the object
//! to point to the twiddle factor table in memory
void FFT32_init(void *);

#ifdef __cplusplus
{
#endif // extern "C"

#endif //_FFT_H_

