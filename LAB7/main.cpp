#include "mbed.h"
#include "stm32l475e_iot01_accelero.h"
#include "arm_math.h"
#include "math_helper.h"
#if defined(SEMIHOSTING)
#include <stdio.h>
#endif
/* ----------------------------------------------------------------------
** Macro Defines
** ------------------------------------------------------------------- */
#define TEST_LENGTH_SAMPLES  320
#define SNR_THRESHOLD_F32    75.0f
#define BLOCK_SIZE            32
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
/* Must be a multiple of 16 */
#define NUM_TAPS_ARRAY_SIZE              32
#else
#define NUM_TAPS_ARRAY_SIZE              29
#endif
#define NUM_TAPS              29
#define SAMPLE_RATE          48000

static float32_t testOutput[TEST_LENGTH_SAMPLES];
/* -------------------------------------------------------------------
 * Declare State buffer of size (numTaps + blockSize - 1)
 * ------------------------------------------------------------------- */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
static float32_t firStateF32[2 * BLOCK_SIZE + NUM_TAPS - 1];
#else
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];
#endif 
/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
const float32_t firCoeffs32[NUM_TAPS_ARRAY_SIZE] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f, 0.0f,0.0f,0.0f
};
#else
const float32_t firCoeffs32[NUM_TAPS_ARRAY_SIZE] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
};
#endif

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;
float32_t  snr;

class DSPDemo {
    public:
        DSPDemo():pDataXYZ{0}{
            memset(output, 0, sizeof(output));
        }

        void gen_sig(float32_t* signal, float32_t freq1, float32_t freq2, int16_t mode){
            for (int i = 0; i < TEST_LENGTH_SAMPLES; i++) {
                if (mode == 0) {
                    float32_t sample1 = arm_sin_f32(2.0 * PI * freq1 * ((float32_t)i / SAMPLE_RATE));
                    float32_t sample2 = arm_sin_f32(2.0 * PI * freq2 * ((float32_t)i / SAMPLE_RATE));
                    signal[i] = 0.5 * (sample1 + sample2);
                } else if (mode == 1){
                    BSP_ACCELERO_AccGetXYZ(pDataXYZ);
                    signal[i] = pDataXYZ[2];
                }
                printf("%.2f\n", signal[i]);
            }
            printf("\nSingal generated\n");

        }
        void run()
        {
            uint32_t i;
            arm_fir_instance_f32 S;
            arm_status status;
            float32_t  *inputF32, *outputF32;
            float32_t testInput[TEST_LENGTH_SAMPLES];

            // 0 for sine wave; 1 for accelerato data
            gen_sig(testInput, 1000, 15000, 1);
            inputF32 = &testInput[0];
            outputF32 = &testOutput[0];
            /* Call FIR init function to initialize the instance structure. */
            arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], blockSize);
            /* ----------------------------------------------------------------------
            ** Call the FIR process function for every blockSize samples
            ** ------------------------------------------------------------------- */
            for(i=0; i < numBlocks; i++)
            {
                arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
            }
            printf("output:\n");
            for (i =0 ;i < TEST_LENGTH_SAMPLES; i++){
                printf("%.4f\n", testOutput[i]);
            }
        }

    private:
        char output[500];
        int16_t pDataXYZ[3];
};

int main() {
    printf("Starting DSP demo\n");
    BSP_ACCELERO_Init();
    DSPDemo demo;
    demo.run();
    return 0;
}