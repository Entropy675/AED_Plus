#ifndef DEFS_H
#define DEFS_H

// Put all of our constant definitions here.

/// CONSTANTS:
#define PING_RATE_MS            10
#define HEART_RATE_SCALE        1.5
#define MINIMUM_RED_COLOR       30
#define HEART_RATE_MON_LOG      true
#define AED_DEMO_LOOP_RATE_MS   500

/// STARTING VALUES:
#define START_HEART_RATE        75


enum AEDState
{ Default,
  AnalyzingResponsiveness,
  EmergencyServices,
  Breathing, // use lid to put behind victims shoulders to maintain an effective airway (dont use support if spinal injury)
  ElectrodePlacement,
  Shock, // shock is also HeartRythmAnalysis,this is where it will be judged
  PostShockCare, // bunched with continued evaluation
}; // we can add more/remove some as we need


#endif // DEFS_H
