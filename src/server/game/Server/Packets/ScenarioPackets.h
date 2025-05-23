/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITYCORE_SCENARIO_PACKETS_H
#define TRINITYCORE_SCENARIO_PACKETS_H

#include "Packet.h"
#include "PacketUtilities.h"
#include "AchievementPackets.h"

#define MAX_ALLOWED_SCENARIO_POI_QUERY_SIZE 66

struct ScenarioPOI;

namespace WorldPackets::Scenario
{
struct BonusObjectiveData
{
    int32 BonusObjectiveID = 0;
    bool ObjectiveComplete = false;
};

struct ScenarioSpellUpdate
{
    uint32 SpellID = 0;
    bool Usable = true;
};

class ScenarioState final : public ServerPacket
{
public:
    explicit ScenarioState() : ServerPacket(SMSG_SCENARIO_STATE) { }

    WorldPacket const* Write() override;

    ObjectGuid ScenarioGUID;
    int32 ScenarioID = 0;
    int32 CurrentStep = -1;
    uint32 DifficultyID = 0;
    uint32 WaveCurrent = 0;
    uint32 WaveMax = 0;
    uint32 TimerDuration = 0;
    std::vector<Achievement::CriteriaProgress> CriteriaProgress;
    std::vector<BonusObjectiveData> BonusObjectives;
    std::vector<uint32> PickedSteps;
    std::vector<ScenarioSpellUpdate> Spells;
    ObjectGuid PlayerGUID;
    bool ScenarioComplete = false;
};

class ScenarioProgressUpdate final : public ServerPacket
{
public:
    explicit ScenarioProgressUpdate() : ServerPacket(SMSG_SCENARIO_PROGRESS_UPDATE) { }

    WorldPacket const* Write() override;

    Achievement::CriteriaProgress CriteriaProgress;
};

class ScenarioCompleted final : public ServerPacket
{
public:
    explicit ScenarioCompleted(uint32 scenarioId) : ServerPacket(SMSG_SCENARIO_COMPLETED, 4), ScenarioID(scenarioId) { }

    WorldPacket const* Write() override;

    uint32 ScenarioID = 0;
};

class ScenarioVacate final : public ServerPacket
{
public:
    explicit ScenarioVacate() : ServerPacket(SMSG_SCENARIO_VACATE, 4 + 4 + 1) { }

    WorldPacket const* Write() override;

    ObjectGuid ScenarioGUID;
    int32 ScenarioID = 0;
    int32 TimeRemain = 0;
    uint8 Reason = 0;
};

class QueryScenarioPOI final : public ClientPacket
{
public:
    explicit QueryScenarioPOI(WorldPacket&& packet) : ClientPacket(CMSG_QUERY_SCENARIO_POI, std::move(packet)) { }

    void Read() override;

    Array<int32, MAX_ALLOWED_SCENARIO_POI_QUERY_SIZE> MissingScenarioPOIs;
};

struct ScenarioPOIData
{
    int32 CriteriaTreeID = 0;
    std::vector<ScenarioPOI> const* ScenarioPOIs = nullptr;
};

class ScenarioPOIs final : public ServerPacket
{
public:
    explicit ScenarioPOIs() : ServerPacket(SMSG_SCENARIO_POIS) { }

    WorldPacket const* Write() override;

    std::vector<ScenarioPOIData> ScenarioPOIDataStats;
};
}

#endif // TRINITYCORE_SCENARIO_PACKETS_H
