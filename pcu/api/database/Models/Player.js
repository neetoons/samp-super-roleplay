module.exports = (sequelize, DataTypes) => {
  return sequelize.define(
    "player",
    {
      id: {
        type: DataTypes.INTEGER,
        allowNull: false,
        autoIncrement: true,
        primaryKey: true,
      },
      name: {
        type: DataTypes.STRING(24),
        allowNull: false,
      },
      ip: {
        type: DataTypes.STRING(16),
        allowNull: false,
      },
      email: {
        type: DataTypes.STRING(32),
        allowNull: false,
      },
      salt: {
        type: DataTypes.STRING(16),
        allowNull: false,
      },
      pass: {
        type: DataTypes.STRING(65),
        allowNull: false,
      },
      reg_date: {
        type: DataTypes.DATE,
        allowNull: false,
        defaultValue: sequelize.literal("CURRENT_TIMESTAMP"),
      },
      last_connection: {
        type: DataTypes.DATE,
        allowNull: false,
        defaultValue: sequelize.literal("CURRENT_TIMESTAMP"),
      },
      last_connection_timestamp: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      time_playing: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      level: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      rep: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      connected: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      playerid: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      doubt_channel: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      time_for_rep: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1500000,
      },
      admin_level: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      payday_rep: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      vip: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      vip_expire_date: {
        type: DataTypes.DATE,
        allowNull: false,
        defaultValue: new Date(0, 0, 0, 0, 0, 0, 0),
      },
      coins: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      skin: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 170,
      },
      cash: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1000,
      },
      pos_x: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: 1773.31,
      },
      pos_y: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: -1896.44,
      },
      pos_z: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: 13.5512,
      },
      angle: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: 270.0,
      },
      state: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      interior: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      local_interior: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      fight_style: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 4,
      },
      health: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: 100.0,
      },
      armour: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: 0.0,
      },
      gender: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      hungry: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: 100.0,
      },
      thirst: {
        type: DataTypes.FLOAT,
        allowNull: false,
        defaultValue: 100.0,
      },
      black_market_level: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      wanted_level: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      police_jail_time: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      police_duty: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      police_jail_id: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      drive_license_points: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      bank_account: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      bank_money: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      phone_number: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      phone_state: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      phone_visible_number: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      gps: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      mp3: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      phone_resolver: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      speakers: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      mechanic_pieces: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      fuel_drum: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      seed_medicine: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      seed_cannabis: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      seed_crack: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      medicine: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      cannabis: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      crack: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      config_sounds: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      config_audio: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      config_time: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      config_hud: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      config_admin: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 1,
      },
      config_secure_login: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      mute: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      placa_pd: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      crew_rank: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      mechanic_kits: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
      secure_login_last_code: {
        type: DataTypes.STRING(16),
        allowNull: false,
        defaultValue: "",
      },
      medical_kits: {
        type: DataTypes.INTEGER,
        allowNull: false,
        defaultValue: 0,
      },
    },
    {
      freezeTableName: true,
      timestamps: false,
    }
  );
};
