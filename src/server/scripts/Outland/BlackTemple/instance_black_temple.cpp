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

#include "ScriptMgr.h"
#include "AreaBoundary.h"
#include "black_temple.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Map.h"

DoorData const doorData[] =
{
    { GO_NAJENTUS_GATE,         DATA_HIGH_WARLORD_NAJENTUS, EncounterDoorBehavior::OpenWhenDone },
    { GO_NAJENTUS_GATE,         DATA_SUPREMUS,              EncounterDoorBehavior::OpenWhenNotInProgress },
    { GO_SUPREMUS_GATE,         DATA_SUPREMUS,              EncounterDoorBehavior::OpenWhenDone },
    { GO_SHADE_OF_AKAMA_DOOR,   DATA_SHADE_OF_AKAMA,        EncounterDoorBehavior::OpenWhenNotInProgress    },
    { GO_TERON_DOOR_1,          DATA_TERON_GOREFIEND,       EncounterDoorBehavior::OpenWhenNotInProgress },
    { GO_TERON_DOOR_2,          DATA_TERON_GOREFIEND,       EncounterDoorBehavior::OpenWhenNotInProgress },
    { GO_GURTOGG_DOOR,          DATA_GURTOGG_BLOODBOIL,     EncounterDoorBehavior::OpenWhenDone },
    { GO_MOTHER_SHAHRAZ_DOOR,   DATA_MOTHER_SHAHRAZ,        EncounterDoorBehavior::OpenWhenDone },
    { GO_COUNCIL_DOOR_1,        DATA_ILLIDARI_COUNCIL,      EncounterDoorBehavior::OpenWhenNotInProgress },
    { GO_COUNCIL_DOOR_2,        DATA_ILLIDARI_COUNCIL,      EncounterDoorBehavior::OpenWhenNotInProgress },
    { GO_ILLIDAN_DOOR_R,        DATA_ILLIDAN_STORMRAGE,     EncounterDoorBehavior::OpenWhenNotInProgress },
    { GO_ILLIDAN_DOOR_L,        DATA_ILLIDAN_STORMRAGE,     EncounterDoorBehavior::OpenWhenNotInProgress },
    { 0,                        0,                          EncounterDoorBehavior::OpenWhenNotInProgress } // END
};

BossBoundaryData const boundaries =
{
    { DATA_HIGH_WARLORD_NAJENTUS, new RectangleBoundary(394.0f, 479.4f, 707.8f, 859.1f)        },
    { DATA_SUPREMUS,              new RectangleBoundary(556.1f, 850.2f, 542.0f, 1001.0f)       },
    { DATA_SHADE_OF_AKAMA,        new RectangleBoundary(406.8f, 564.0f, 327.9f, 473.5f)        },
    { DATA_TERON_GOREFIEND,       new RectangleBoundary(512.5f, 613.3f, 373.2f, 432.0f)        },
    { DATA_TERON_GOREFIEND,       new ZRangeBoundary(179.5f, 223.6f)                           },
    { DATA_GURTOGG_BLOODBOIL,     new RectangleBoundary(720.5f, 864.5f, 159.3f, 316.0f)        },
    { DATA_RELIQUARY_OF_SOULS,    new RectangleBoundary(435.9f, 660.3f, 21.2f, 229.6f)         },
    { DATA_RELIQUARY_OF_SOULS,    new ZRangeBoundary(81.8f, 148.0f)                            },
    { DATA_MOTHER_SHAHRAZ,        new RectangleBoundary(903.4f, 982.1f, 92.4f, 313.2f)         },
    { DATA_ILLIDARI_COUNCIL,      new EllipseBoundary(Position(696.6f, 305.0f), 70.0f , 85.0f) },
    { DATA_ILLIDAN_STORMRAGE,     new EllipseBoundary(Position(694.8f, 309.0f), 80.0f , 95.0f) }
};

ObjectData const creatureData[] =
{
    { NPC_HIGH_WARLORD_NAJENTUS,        DATA_HIGH_WARLORD_NAJENTUS      },
    { NPC_SUPREMUS,                     DATA_SUPREMUS                   },
    { NPC_SHADE_OF_AKAMA,               DATA_SHADE_OF_AKAMA             },
    { NPC_TERON_GOREFIEND,              DATA_TERON_GOREFIEND            },
    { NPC_GURTOGG_BLOODBOIL,            DATA_GURTOGG_BLOODBOIL          },
    { NPC_RELIQUARY_OF_SOULS,           DATA_RELIQUARY_OF_SOULS         },
    { NPC_MOTHER_SHAHRAZ,               DATA_MOTHER_SHAHRAZ             },
    { NPC_ILLIDARI_COUNCIL,             DATA_ILLIDARI_COUNCIL           },
    { NPC_ILLIDAN_STORMRAGE,            DATA_ILLIDAN_STORMRAGE          },
    { NPC_AKAMA_SHADE,                  DATA_AKAMA_SHADE                },
    { NPC_AKAMA,                        DATA_AKAMA                      },
    { NPC_GATHIOS_THE_SHATTERER,        DATA_GATHIOS_THE_SHATTERER      },
    { NPC_HIGH_NETHERMANCER_ZEREVOR,    DATA_HIGH_NETHERMANCER_ZEREVOR  },
    { NPC_LADY_MALANDE,                 DATA_LADY_MALANDE               },
    { NPC_VERAS_DARKSHADOW,             DATA_VERAS_DARKSHADOW           },
    { NPC_BLOOD_ELF_COUNCIL_VOICE,      DATA_BLOOD_ELF_COUNCIL_VOICE    },
    { NPC_BLACK_TEMPLE_TRIGGER,         DATA_BLACK_TEMPLE_TRIGGER       },
    { NPC_MAIEV_SHADOWSONG,             DATA_MAIEV                      },
    { NPC_RELIQUARY_COMBAT_TRIGGER,     DATA_RELIQUARY_COMBAT_TRIGGER   },
    { 0,                                0                               } // END
};

ObjectData const gameObjectData[] =
{
    { GO_ILLIDAN_GATE,                DATA_GO_ILLIDAN_GATE          },
    { GO_DEN_OF_MORTAL_DOOR,          DATA_GO_DEN_OF_MORTAL_DOOR    },
    { GO_ILLIDAN_MUSIC_CONTROLLER,    DATA_ILLIDAN_MUSIC_CONTROLLER },
    { 0,                              0                             } //END
};

DungeonEncounterData const encounters[] =
{
    { DATA_HIGH_WARLORD_NAJENTUS, {{ 601 }} },
    { DATA_SUPREMUS, {{ 602 }} },
    { DATA_SHADE_OF_AKAMA, {{ 603 }} },
    { DATA_TERON_GOREFIEND, {{ 604 }} },
    { DATA_GURTOGG_BLOODBOIL, {{ 605 }} },
    { DATA_RELIQUARY_OF_SOULS, {{ 606 }} },
    { DATA_MOTHER_SHAHRAZ, {{ 607 }} },
    { DATA_ILLIDARI_COUNCIL, {{ 608 }} },
    { DATA_ILLIDAN_STORMRAGE, {{ 609 }} }
};

class instance_black_temple : public InstanceMapScript
{
    public:
        instance_black_temple() : InstanceMapScript(BTScriptName, 564) { }

        struct instance_black_temple_InstanceMapScript : public InstanceScript
        {
            instance_black_temple_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);
                LoadObjectData(creatureData, gameObjectData);
                LoadBossBoundaries(boundaries);
                LoadDungeonEncounterData(encounters);
                AkamaState = AKAMA_INTRO;
                AkamaIllidanIntro = 1;
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                InstanceScript::OnGameObjectCreate(go);

                if (go->GetEntry() == GO_DEN_OF_MORTAL_DOOR)
                    if (CheckDenOfMortalDoor())
                        HandleGameObject(ObjectGuid::Empty, true, go);
            }

            void OnCreatureCreate(Creature* creature) override
            {
                InstanceScript::OnCreatureCreate(creature);

                switch (creature->GetEntry())
                {
                    case NPC_ASHTONGUE_STALKER:
                    case NPC_ASHTONGUE_BATTLELORD:
                    case NPC_ASHTONGUE_MYSTIC:
                    case NPC_ASHTONGUE_PRIMALIST:
                    case NPC_ASHTONGUE_STORMCALLER:
                    case NPC_ASHTONGUE_FERAL_SPIRIT:
                    case NPC_STORM_FURY:
                        AshtongueGUIDs.push_back(creature->GetGUID());
                        if (GetBossState(DATA_SHADE_OF_AKAMA) == DONE)
                            creature->SetFaction(FACTION_ASHTONGUE_DEATHSWORN);
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_AKAMA:
                        return AkamaState;
                    case DATA_AKAMA_ILLIDAN_INTRO:
                        return AkamaIllidanIntro;
                    default:
                        return 0;
                }
            }

            void SetData(uint32 type, uint32 data) override
            {
                switch (type)
                {
                    case DATA_AKAMA:
                        AkamaState = data;
                        break;
                    case ACTION_OPEN_DOOR:
                        if (GameObject* illidanGate = GetGameObject(DATA_GO_ILLIDAN_GATE))
                            HandleGameObject(ObjectGuid::Empty, true, illidanGate);
                        break;
                    case DATA_AKAMA_ILLIDAN_INTRO:
                        AkamaIllidanIntro = data;
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_HIGH_WARLORD_NAJENTUS:
                        if (state == DONE)
                            if (Creature* trigger = GetCreature(DATA_BLACK_TEMPLE_TRIGGER))
                                trigger->AI()->Talk(EMOTE_HIGH_WARLORD_NAJENTUS_DIED);
                        break;
                    case DATA_SHADE_OF_AKAMA:
                        if (state == DONE)
                            for (ObjectGuid ashtongueGuid : AshtongueGUIDs)
                                if (Creature* ashtongue = instance->GetCreature(ashtongueGuid))
                                    ashtongue->SetFaction(FACTION_ASHTONGUE_DEATHSWORN);
                        [[fallthrough]];
                    case DATA_TERON_GOREFIEND:
                    case DATA_GURTOGG_BLOODBOIL:
                    case DATA_RELIQUARY_OF_SOULS:
                        if (state == DONE && CheckDenOfMortalDoor())
                        {
                            if (Creature* trigger = GetCreature(DATA_BLACK_TEMPLE_TRIGGER))
                                trigger->AI()->Talk(EMOTE_DEN_OF_MORTAL_DOOR_OPEN);

                            if (GameObject* door = GetGameObject(DATA_GO_DEN_OF_MORTAL_DOOR))
                                HandleGameObject(ObjectGuid::Empty, true, door);
                        }
                        break;
                    case DATA_ILLIDARI_COUNCIL:
                        if (state == DONE)
                            if (Creature* akama = GetCreature(DATA_AKAMA))
                                akama->AI()->DoAction(ACTION_ACTIVE_AKAMA_INTRO);
                        break;
                    default:
                        break;
                }

                return true;
            }

            bool CheckDenOfMortalDoor()
            {
                for (BTDataTypes boss : {DATA_SHADE_OF_AKAMA, DATA_TERON_GOREFIEND, DATA_RELIQUARY_OF_SOULS, DATA_GURTOGG_BLOODBOIL})
                    if (GetBossState(boss) != DONE)
                        return false;
                return true;
            }

        protected:
            GuidVector AshtongueGUIDs;
            uint8 AkamaState;
            uint8 AkamaIllidanIntro;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_black_temple_InstanceMapScript(map);
        }
};

void AddSC_instance_black_temple()
{
    new instance_black_temple();
}
