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

#ifndef CharacterCache_h__
#define CharacterCache_h__

#include "Define.h"
#include "ObjectGuid.h"
#include "Optional.h"
#include "SharedDefines.h"
#include <string>

struct CharacterCacheEntry
{
    ObjectGuid Guid;
    std::string Name;
    uint32 AccountId;
    uint8 Class;
    uint8 Race;
    uint8 Sex;
    uint8 Level;
    ObjectGuid::LowType GuildId;
    uint32 ArenaTeamId[3];
    bool IsDeleted;
};

class TC_GAME_API CharacterCache
{
    public:
        CharacterCache();
        CharacterCache(CharacterCache const&) = delete;
        CharacterCache(CharacterCache&&) = delete;
        CharacterCache& operator=(CharacterCache const&) = delete;
        CharacterCache& operator=(CharacterCache&&) = delete;
        ~CharacterCache();
        static CharacterCache* instance();

        void LoadCharacterCacheStorage();
        void AddCharacterCacheEntry(ObjectGuid const& guid, uint32 accountId, std::string const& name, uint8 gender, uint8 race, uint8 playerClass, uint8 level, bool isDeleted);
        void DeleteCharacterCacheEntry(ObjectGuid const& guid, std::string const& name);

        void UpdateCharacterData(ObjectGuid const& guid, std::string const& name, Optional<uint8> gender = {}, Optional<uint8> race = {});
        void UpdateCharacterGender(ObjectGuid const& guid, uint8 gender);
        void UpdateCharacterLevel(ObjectGuid const& guid, uint8 level);
        void UpdateCharacterAccountId(ObjectGuid const& guid, uint32 accountId);
        void UpdateCharacterGuildId(ObjectGuid const& guid, ObjectGuid::LowType guildId);
        void UpdateCharacterArenaTeamId(ObjectGuid const& guid, uint8 slot, uint32 arenaTeamId);
        void UpdateCharacterInfoDeleted(ObjectGuid const& guid, bool deleted, std::string const& name);

        bool HasCharacterCacheEntry(ObjectGuid const& guid) const;
        CharacterCacheEntry const* GetCharacterCacheByGuid(ObjectGuid const& guid) const;
        CharacterCacheEntry const* GetCharacterCacheByName(std::string const& name) const;

        ObjectGuid GetCharacterGuidByName(std::string const& name) const;
        bool GetCharacterNameByGuid(ObjectGuid guid, std::string& name) const;
        Team GetCharacterTeamByGuid(ObjectGuid guid) const;
        uint32 GetCharacterAccountIdByGuid(ObjectGuid guid) const;
        uint32 GetCharacterAccountIdByName(std::string const& name) const;
        uint8 GetCharacterLevelByGuid(ObjectGuid guid) const;
        ObjectGuid::LowType GetCharacterGuildIdByGuid(ObjectGuid guid) const;
        uint32 GetCharacterArenaTeamIdByGuid(ObjectGuid guid, uint8 type) const;
        bool GetCharacterNameAndClassByGUID(ObjectGuid guid, std::string& name, uint8& _class) const;
};

#define sCharacterCache CharacterCache::instance()

#endif // CharacterCache_h__
