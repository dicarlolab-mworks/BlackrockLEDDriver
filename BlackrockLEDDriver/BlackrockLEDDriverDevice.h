//
//  BlackrockLEDDriverDevice.h
//  BlackrockLEDDriver
//
//  Created by Christopher Stawarz on 9/24/14.
//  Copyright (c) 2014 The MWorks Project. All rights reserved.
//

#ifndef __BlackrockLEDDriver__BlackrockLEDDriverDevice__
#define __BlackrockLEDDriver__BlackrockLEDDriverDevice__

#include "BlackrockLEDDriverCommand.h"


BEGIN_NAMESPACE_MW_BLACKROCK_LEDDRIVER


class Device : public IODevice, boost::noncopyable {
    
public:
    static const std::string RUNNING;
    static const std::string TEMP_A;
    static const std::string TEMP_B;
    static const std::string TEMP_C;
    static const std::string TEMP_D;
    static const std::string TEMP_CALC;
    static const std::string SIMULATE_DEVICE;
    
    static void describeComponent(ComponentInfo &info);
    
    explicit Device(const ParameterValueMap &parameters);
    ~Device();
    
    bool initialize() override;
    bool stopDeviceIO() override;
    
    void setIntensity(const std::set<int> &channels, double value);
    void prepare(MWTime duration);
    void run(MWTime duration);
    void stop();
    void readTemps();
    
private:
    bool updateFile(MWTime duration);
    bool quantizeDuration(MWTime duration, WORD &period, std::size_t &samplesUsed);
    bool setFileTimePeriod(WORD period);
    bool loadFile(std::size_t samplesUsed);
    bool startFilePlaying();
    bool checkIfFileStopped();
    bool stopFilePlaying();
    
    template<typename Request, typename Response>
    bool perform(Request &request, Response &response) { return request.write(handle) && response.read(handle); }
    
    template<typename Message>
    bool perform(Message &message) { return perform(message, message); }
    
    const VariablePtr running;
    const VariablePtr tempA;
    const VariablePtr tempB;
    const VariablePtr tempC;
    const VariablePtr tempD;
    const VariablePtr tempCalc;
    const bool simulateDevice;
    
    const boost::shared_ptr<Clock> clock;
    
    FT_HANDLE handle;
    std::array<WordValue, numChannels> intensity;
    
    boost::shared_ptr<ScheduleTask> checkStatusTask;
    
    std::mutex mutex;
    using lock_guard = std::lock_guard<std::mutex>;
    
    bool intensityChanged;
    bool filePlaying;
    MWTime lastRunDuration;
    
    MWTime simulatedRunStartTime;
    
};


END_NAMESPACE_MW_BLACKROCK_LEDDRIVER


#endif /* !defined(__BlackrockLEDDriver__BlackrockLEDDriverDevice__) */
