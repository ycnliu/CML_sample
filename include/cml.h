#ifndef CML_H
#define CML_H

#include <string>

namespace CML {

  enum HomeMethod {
    CHM_NONE,
    CHM_INDEX_POS,
    CHM_SWITCH,
    // Add others as needed
};

  // Homing configuration struct
  struct HomeConfig {
      HomeMethod method;
      int offset;
      int velFast;
      int velSlow;

      HomeConfig()
          : method(CHM_NONE), offset(0), velFast(10000), velSlow(1000) {}
  };


    // Error class
    class Error {
    public:
        int code;
        std::string msg;
        Error(int c, const std::string& m) : code(c), msg(m) {}
        const char* toString() const { return msg.c_str(); }
    };

    static const Error* const SUCCESS = nullptr;

    // Simulated network
    class Network {
    public:
        bool opened;
        Network() : opened(false) {}
        const Error* Open() {
            opened = true;
            return SUCCESS;
        }
    };

    // Amplifier settings placeholder
    class AmpSettings {
    public:
        int synchPeriod;
        int guardTime;
        AmpSettings() : synchPeriod(1000), guardTime(0) {}
    };

    // Amplifier
    class Amp {
    private:
        bool initialized;
        double position;
        double velocity;
        int axisID;

    public:
        Amp() : initialized(false), position(0.0), velocity(0.0), axisID(0) {}

        const Error* Init(Network& net, int nodeID, const AmpSettings& settings = AmpSettings()) {
            if (!net.opened) {
                static Error errNet(-1, "Network not opened");
                return &errNet;
            }
            initialized = true;
            axisID = nodeID;
            (void)settings;
            return SUCCESS;
        }

        const Error* InitSubAxis(Amp& masterAxis, int subAxisNumber, const AmpSettings& settings = AmpSettings()) {
            if (!masterAxis.initialized) {
                static Error errMaster(-2, "Master axis not initialized");
                return &errMaster;
            }
            initialized = true;
            axisID = subAxisNumber;
            (void)settings;
            return SUCCESS;
        }

        const Error* MoveAbs(double targetPosition) {
            if (!initialized) {
                static Error errAxis(-3, "Axis not initialized");
                return &errAxis;
            }
            position = targetPosition;
            velocity = 0.0;
            return SUCCESS;
        }

        const Error* MoveRel(double offset) {
            if (!initialized) {
                static Error errAxis(-3, "Axis not initialized");
                return &errAxis;
            }
            position += offset;
            velocity = 0.0;
            return SUCCESS;
        }

        const Error* MoveVel(double vel) {
            if (!initialized) {
                static Error errAxis(-3, "Axis not initialized");
                return &errAxis;
            }
            velocity = vel;
            return SUCCESS;
        }

        const Error* Stop() {
            if (!initialized) {
                static Error errAxis(-3, "Axis not initialized");
                return &errAxis;
            }
            velocity = 0.0;
            return SUCCESS;
        }

        double GetPosition() const {
            return position;
        }

        void SetPosition(double newPos) {
            position = newPos;
        }

        bool IsInitialized() const {
            return initialized;
        }
        const Error* GoHome(const HomeConfig& cfg) {
          if (!initialized) {
              static Error errAxis(-4, "Axis not initialized");
              return &errAxis;
          }
          // In a real system, configure homing trajectory using cfg.
          // Here we just simulate that position is reset to zero.
          position = 0.0;
          velocity = 0.0;
          return SUCCESS;
      }
      
      // Static method to wait for homing (simulated)
      static const Error* WaitMoveDone(Amp* axes, int count, int timeoutMs) {
          for (int i = 0; i < count; ++i) {
              if (!axes[i].IsInitialized()) {
                  static Error errWait(-5, "Axis not initialized during wait");
                  return &errWait;
              }
          }
          // Simulate a delay or success
          return SUCCESS;
      }
    };

} // namespace CML

#endif // CML_H
