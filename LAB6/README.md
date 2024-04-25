Q1. By modify prescaler and period, we can adjust the trigger frequency and sampling time of ADC.

    htim1.Init.Prescaler = 4000 - 1;
    htim1.Init.Period = 50000 - 1;  // origin: 1000 -1

Q2. Add the following functions:

    void temp_half_callback()
    void temp_full_callback()
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)

Q3. Add the following functions:

    void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
    void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)


