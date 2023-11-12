class KalmanFilter
{
public:
    KalmanFilter(float initial_state, float initial_covariance, float process_noise, float measurement_noise)
    {
        state = initial_state;
        covariance = initial_covariance;
        Q = process_noise;
        R = measurement_noise;
    }

    float predict()
    {
        // Prediction step: Project the state forward
        // Here, we assume no control input, so we omit the control matrix
        state = state; // In a more complex model, you can add motion models here
        covariance += Q;

        return state;
    }

    float update(float measurement)
    {
        // Update step: Correct the predicted state using the measurement
        float kalman_gain = covariance / (covariance + R);
        state += kalman_gain * (measurement - state);
        covariance *= (1 - kalman_gain);

        return state;
    }

    float getState()
    {
        return state;
    }

private:
    float state;
    float covariance;
    float Q; // Process noise covariance
    float R; // Measurement noise covariance
};

float initial_state = 0;
float initial_covariance = 1;
float process_noise = 0.1;
float measurement_noise = 2.0;

KalmanFilter kf(initial_state, initial_covariance, process_noise, measurement_noise);

float kalmanMPU6050(float mpu6050_data)
{
    kf.predict();
    float data = kf.update(mpu6050_data);
    return data;
}