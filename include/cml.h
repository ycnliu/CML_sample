#ifndef CML_SIM_H
#define CML_SIM_H

#include <string>
#include <vector>
#include <iostream>

namespace CML {

  enum HomeMethod {
    CHM_NONE,
    CHM_INDEX_POS,
    CHM_SWITCH
  };

  struct HomeConfig {
    HomeMethod method;
    int offset;
    int velFast;
    int velSlow;
    int accel;
    int timeout;

    HomeConfig()
        : method(CHM_NONE), offset(0),
          velFast(10000), velSlow(1000),
          accel(5000), timeout(5000) {}
  };

  class Error {
  public:
    int code;
    std::string msg;

    Error(int c, const std::string& m) : code(c), msg(m) {}
    const char* toString() const { return msg.c_str(); }
  };

  static const Error* const SUCCESS = nullptr;

  class Network {
  public:
    bool opened;
    Network() : opened(false) {}
    const Error* Open() {
      opened = true;
      return SUCCESS;
    }
  };

  class AmpSettings {
  public:
    int synchPeriod;
    int guardTime;
    AmpSettings() : synchPeriod(1000), guardTime(0) {}
  };

  class Amp {
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

    const Error* GoHome(const HomeConfig& cfg) {
      if (!initialized) {
        static Error errAxis(-4, "Axis not initialized");
        return &errAxis;
      }
      position = 0.0;
      velocity = 0.0;
      return SUCCESS;
    }

    static const Error* WaitMoveDone(Amp* axes, int count, int timeoutMs) {
      for (int i = 0; i < count; ++i) {
        if (!axes[i].IsInitialized()) {
          static Error errWait(-5, "Axis not initialized during wait");
          return &errWait;
        }
      }
      return SUCCESS;
    }

    double GetPosition() const { return position; }
    void SetPosition(double newPos) { position = newPos; }
    bool IsInitialized() const { return initialized; }
  };

  class Path {
    int numAxes;
    std::vector<std::vector<double>> segments;
  public:
    Path(int axes) : numAxes(axes) {}

    void SetStartPos(const double* pos) {
      segments.clear();
      std::vector<double> start(pos, pos + numAxes);
      segments.push_back(start);
    }

    void AddLine(const double* pos) {
      std::vector<double> end(pos, pos + numAxes);
      segments.push_back(end);
    }

    void AddArc(const double* center, double angle) {
      std::vector<double> arc(center, center + numAxes);
      arc.push_back(angle);  // fake storage
      segments.push_back(arc);
    }
  };

  class Linkage {
    Amp* amps;
    int num;
    double vel, acc, dec, jerk;
  public:
    Linkage() : amps(nullptr), num(0), vel(0), acc(0), dec(0), jerk(0) {}

    const Error* Init(Amp* list, int n) {
      amps = list;
      num = n;
      return SUCCESS;
    }

    void SetMoveLimits(double v, double a, double d, double j) {
      vel = v; acc = a; dec = d; jerk = j;
    }

    const Error* SendTrajectory(const Path& path, bool blocking = true) {
      return SUCCESS;
    }
  };

  class CopleyMotionLibraries {
  public:
    enum LogLevel { LOG_NONE, LOG_ERRORS, LOG_DEBUG };
    void SetDebugLevel(LogLevel lvl) {
      switch (lvl) {
        case LOG_NONE: break;
        case LOG_ERRORS: std::cout << "Debug: LOG_ERRORS\n"; break;
        case LOG_DEBUG: std::cout << "Debug: LOG_DEBUG\n"; break;
      }
    }
  };

  static CopleyMotionLibraries cml;

  class LinuxEcatHardware {};
  class EtherCAT {
  public:
    const Error* Open(LinuxEcatHardware& hw) { return SUCCESS; }
  };

} // namespace CML

#endif // CML_SIM_H
