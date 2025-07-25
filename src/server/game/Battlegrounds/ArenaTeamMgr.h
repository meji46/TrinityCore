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

#ifndef _ARENATEAMMGR_H
#define _ARENATEAMMGR_H

#include "ArenaTeam.h"
#include <unordered_map>

class TC_GAME_API ArenaTeamMgr
{
private:
    ArenaTeamMgr();
    ~ArenaTeamMgr();

public:
    ArenaTeamMgr(ArenaTeamMgr const&) = delete;
    ArenaTeamMgr(ArenaTeamMgr&&) = delete;
    ArenaTeamMgr& operator=(ArenaTeamMgr const&) = delete;
    ArenaTeamMgr& operator=(ArenaTeamMgr&&) = delete;

    static ArenaTeamMgr* instance();

    typedef std::unordered_map<uint32, ArenaTeam*> ArenaTeamContainer;

    ArenaTeam* GetArenaTeamById(uint32 arenaTeamId) const;
    ArenaTeam* GetArenaTeamByName(std::string_view arenaTeamName) const;
    ArenaTeam* GetArenaTeamByCaptain(ObjectGuid guid) const;

    void LoadArenaTeams();
    void AddArenaTeam(ArenaTeam* arenaTeam);
    void RemoveArenaTeam(uint32 Id);

    ArenaTeamContainer const& GetArenaTeams() const { return ArenaTeamStore; }

    uint32 GenerateArenaTeamId();
    void SetNextArenaTeamId(uint32 Id) { NextArenaTeamId = Id; }

protected:
    uint32 NextArenaTeamId;
    ArenaTeamContainer ArenaTeamStore;
};

#define sArenaTeamMgr ArenaTeamMgr::instance()

#endif
