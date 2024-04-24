const { sequelize } = require("../connection");
const PlayerModel = require("../model/Player"),
  CrewModel = require("../model/Crew"),
  CrewRanksModel = require("../model/CrewRanks"),
  BadHistoryModel = require("../model/BadHistory"),
  BankMovementsModel = require("../model/BankMovements"),
  PbookModel = require("../model/Pbook"),
  PfoodsModel = require("../model/Pfoods"),
  PgpsModel = require("../model/Pgps"),
  PmessagesModel = require("../model/Pmessages"),
  PoliceHistoryModel = require("../model/PoliceHistory"),
  PtoysModel = require("../model/Ptoys"),
  PvehiclesModel = require("../model/Pvehicles"),
  PweaponsModel = require("../model/Pweapons"),
  ShopModel = require("../model/Shop"),
  TerritoriesModel = require("../model/Territories"),
  VbootModel = require("../model/Vboot"),
  VcomponentsModel = require("../model/Vcomponents"),
  VcomponentsInfoModel = require("../model/VcomponentsInfo"),
  VobjectsModel = require("../model/Vobjects"),
  WorksModel = require("../model/Works"),
  BansModel = require("../model/Bans"),
  PropertiesModel = require("../model/Properties"),
  PropertyClosetModel = require("../model/PropertyCloset"),
  PropertyObjectsModel = require("../model/PropertyObjects"),
  PworksModel = require("../model/Pworks"),
  PworksPointsModel = require("../model/PworksPoints"),
  WebStoreProductsModel = require("../model/WebStoreProducts"),
  WebStoreHistoryModel = require("../model/WebStoreHistory"),
  PlayerSecureLoginModel = require("../model/PlayerSecureLogin"),
  GraffitiModel = require("../model/Graffiti");

const Player = PlayerModel(sequelize, Sequelize),
  Crew = CrewModel(sequelize, Sequelize),
  CrewRanks = CrewRanksModel(sequelize, Sequelize),
  BadHistory = BadHistoryModel(sequelize, Sequelize),
  BankMovements = BankMovementsModel(sequelize, Sequelize),
  Pbook = PbookModel(sequelize, Sequelize),
  Pfoods = PfoodsModel(sequelize, Sequelize),
  Pgps = PgpsModel(sequelize, Sequelize),
  Pmessages = PmessagesModel(sequelize, Sequelize),
  PoliceHistory = PoliceHistoryModel(sequelize, Sequelize),
  Ptoys = PtoysModel(sequelize, Sequelize),
  Pvehicles = PvehiclesModel(sequelize, Sequelize),
  Pweapons = PweaponsModel(sequelize, Sequelize),
  Shop = ShopModel(sequelize, Sequelize),
  Territories = TerritoriesModel(sequelize, Sequelize),
  Vboot = VbootModel(sequelize, Sequelize),
  Vcomponents = VcomponentsModel(sequelize, Sequelize),
  VcomponentsInfo = VcomponentsInfoModel(sequelize, Sequelize),
  Vobjects = VobjectsModel(sequelize, Sequelize),
  Works = WorksModel(sequelize, Sequelize),
  Bans = BansModel(sequelize, Sequelize),
  Properties = PropertiesModel(sequelize, Sequelize),
  PropertyCloset = PropertyClosetModel(sequelize, Sequelize),
  PropertyObjects = PropertyObjectsModel(sequelize, Sequelize),
  Pworks = PworksModel(sequelize, Sequelize),
  PworksPoints = PworksPointsModel(sequelize, Sequelize),
  WebStoreProducts = WebStoreProductsModel(sequelize, Sequelize),
  WebStoreHistory = WebStoreHistoryModel(sequelize, Sequelize),
  PlayerSecureLogin = PlayerSecureLoginModel(sequelize, Sequelize),
  Graffiti = GraffitiModel(sequelize, Sequelize);
// --- Associations ---
Player.belongsTo(Crew, {
  foreignKey: {
    name: "crew",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
  as: "playerCrew",
});

CrewRanks.belongsTo(Crew, {
  foreignKey: {
    name: "id_crew",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

BadHistory.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

BankMovements.belongsTo(Player, {
  foreignKey: {
    name: "from_id",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
  as: "fromId",
});

BankMovements.belongsTo(Player, {
  foreignKey: {
    name: "to_id",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
  as: "toId",
});

Pbook.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onUpdate: "CASCADE",
  onDelete: "CASCADE",
});

Pfoods.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Pgps.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Pmessages.belongsTo(Player, {
  foreignKey: {
    name: "from_id",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
  as: "fromId",
});

Pmessages.belongsTo(Player, {
  foreignKey: {
    name: "to_id",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
  as: "toId",
});

PoliceHistory.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

PoliceHistory.belongsTo(Player, {
  foreignKey: {
    name: "by_id",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
});

Ptoys.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Pvehicles.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Pweapons.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Shop.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Territories.belongsTo(Crew, {
  foreignKey: {
    name: "id_crew",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
});

Vboot.belongsTo(Pvehicles, {
  foreignKey: {
    name: "id_vehicle",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Vobjects.belongsTo(Pvehicles, {
  foreignKey: {
    name: "id_vehicle",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Bans.belongsTo(BadHistory, {
  foreignKey: {
    name: "id_history",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
});

Properties.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
});

Properties.belongsTo(Territories, {
  foreignKey: {
    name: "id_territory",
    allowNull: true,
  },
  onDelete: "SET NULL",
  onUpdate: "CASCADE",
});

PropertyCloset.belongsTo(Properties, {
  foreignKey: {
    name: "id_property",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

PropertyObjects.belongsTo(Properties, {
  foreignKey: {
    name: "id_property",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Player.belongsToMany(Works, {
  through: Pworks,
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Works.belongsToMany(Player, {
  through: Pworks,
  foreignKey: {
    name: "id_work",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Player.belongsToMany(Works, {
  through: PworksPoints,
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Works.belongsToMany(Player, {
  through: PworksPoints,
  foreignKey: {
    name: "id_work",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

WebStoreHistory.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

WebStoreHistory.belongsTo(WebStoreProducts, {
  foreignKey: {
    name: "id_product",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

PlayerSecureLogin.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
    primaryKey: true,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Graffiti.belongsTo(Territories, {
  foreignKey: {
    name: "id_territory",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Graffiti.belongsTo(Crew, {
  foreignKey: {
    name: "id_crew",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});

Graffiti.belongsTo(Player, {
  foreignKey: {
    name: "id_player",
    allowNull: false,
  },
  onDelete: "CASCADE",
  onUpdate: "CASCADE",
});
// ---

module.exports = {
  Player,
  Crew,
  CrewRanks,
  BadHistory,
  BankMovements,
  Pbook,
  Pfoods,
  Pgps,
  Pmessages,
  PoliceHistory,
  Ptoys,
  Pvehicles,
  Pweapons,
  Shop,
  Territories,
  Vboot,
  Vcomponents,
  VcomponentsInfo,
  Vobjects,
  Works,
  Bans,
  Properties,
  PropertyCloset,
  PropertyObjects,
  Pworks,
  PworksPoints,
  WebStoreProducts,
  WebStoreHistory,
  PlayerSecureLogin,
  Graffiti,
};
