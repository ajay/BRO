#ifndef HEADER_PID_CLASS
  #define HEADER_PID_CLASS
   
  class PID {
    public:
      PID(float kP, float kI, float kD, int (*errorFunc)( void ), int (*sensorFunc)( void ), bool (*finishFunc)( int ));
      int getCurrSpeed();
      void reset();
      void setBaseVel(double new_vel);
      bool isFinished();
    private:
      // Limit function
      float limit(float x, float a, float b);

      int finishTimer;
      int finishCount;

      double base_vel;

      float error;
      float error_sum;
      float prev_error;
      float error_diff;

      float kP;
      float kI;
      float kD;

      int (*errorFunc)( void );
      int (*sensorFunc)( void ); 
      bool (*finishFunc)( int );
  };
   
#endif
