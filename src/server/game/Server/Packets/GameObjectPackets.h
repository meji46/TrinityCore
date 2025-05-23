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

#ifndef TRINITYCORE_GAME_OBJECT_PACKETS_H
#define TRINITYCORE_GAME_OBJECT_PACKETS_H

#include "ObjectGuid.h"
#include "Packet.h"

enum class PlayerInteractionType : int32;

namespace WorldPackets
{
    namespace GameObject
    {
        class GameObjUse final : public ClientPacket
        {
        public:
            explicit GameObjUse(WorldPacket&& packet) : ClientPacket(CMSG_GAME_OBJ_USE, std::move(packet)) { }

            void Read() override;

            ObjectGuid Guid;
        };

        class GameObjReportUse final : public ClientPacket
        {
        public:
            explicit GameObjReportUse(WorldPacket&& packet) : ClientPacket(CMSG_GAME_OBJ_REPORT_USE, std::move(packet)) { }

            void Read() override;

            ObjectGuid Guid;
        };

        class GameObjectDespawn final : public ServerPacket
        {
        public:
            explicit GameObjectDespawn() : ServerPacket(SMSG_GAME_OBJECT_DESPAWN, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid ObjectGUID;
        };

        class PageText final : public ServerPacket
        {
        public:
            explicit PageText() : ServerPacket(SMSG_PAGE_TEXT, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid GameObjectGUID;
        };

        class GameObjectActivateAnimKit final : public ServerPacket
        {
        public:
            explicit GameObjectActivateAnimKit() : ServerPacket(SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT, 16 + 4 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid ObjectGUID;
            int32 AnimKitID = 0;
            bool Maintain = false;
        };

        class DestructibleBuildingDamage final : public ServerPacket
        {
        public:
            explicit DestructibleBuildingDamage() : ServerPacket(SMSG_DESTRUCTIBLE_BUILDING_DAMAGE, 16 + 16 + 16 + 4 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid Target;
            ObjectGuid Caster;
            ObjectGuid Owner;
            int32 Damage = 0;
            int32 SpellID = 0;
        };

        class FishNotHooked final : public ServerPacket
        {
        public:
            explicit FishNotHooked() : ServerPacket(SMSG_FISH_NOT_HOOKED, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class FishEscaped final : public ServerPacket
        {
        public:
            explicit FishEscaped() : ServerPacket(SMSG_FISH_ESCAPED, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class GameObjectCustomAnim final : public ServerPacket
        {
        public:
            explicit GameObjectCustomAnim() : ServerPacket(SMSG_GAME_OBJECT_CUSTOM_ANIM, 16 + 4 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid ObjectGUID;
            uint32 CustomAnim = 0;
            bool PlayAsDespawn = false;
        };

        class GameObjectPlaySpellVisual final : public ServerPacket
        {
        public:
            explicit GameObjectPlaySpellVisual() : ServerPacket(SMSG_GAME_OBJECT_PLAY_SPELL_VISUAL, 16 + 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid ObjectGUID;
            ObjectGuid ActivatorGUID;
            int32 SpellVisualID = 0;
        };

        class GameObjectSetStateLocal final : public ServerPacket
        {
        public:
            explicit GameObjectSetStateLocal() : ServerPacket(SMSG_GAME_OBJECT_SET_STATE_LOCAL, 16 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid ObjectGUID;
            uint8 State = 0;
        };

        class GameObjectInteraction final : public ServerPacket
        {
        public:
            explicit GameObjectInteraction() : ServerPacket(SMSG_GAME_OBJECT_INTERACTION, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid ObjectGUID;
            PlayerInteractionType InteractionType = {};
        };

        class GameObjectCloseInteraction final : public ServerPacket
        {
        public:
            explicit GameObjectCloseInteraction() : ServerPacket(SMSG_GAME_OBJECT_CLOSE_INTERACTION, 4) { }

            WorldPacket const* Write() override;

            PlayerInteractionType InteractionType = {};
        };
    }
}

#endif // TRINITYCORE_GAME_OBJECT_PACKETS_H
