module.exports = (sequelize, DataTypes) => {
    return sequelize.define("pvehicles",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            plate: {
                type: DataTypes.STRING(32),
                allowNull: true
            },
            modelid: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            spawn_x: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            spawn_y: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            spawn_z: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            spawn_angle: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            health: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 1000.0
            },
            damage_panels: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            damage_doors: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            damage_lights: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            damage_tires: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            color1: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 1
            },
            color2: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 1
            },
            paintjob: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 3
            },
            gas: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 100.0
            },
            max_gas: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 100.0
            },
            closed: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            interior: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            world: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            state: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot0: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot1: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot2: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot3: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot4: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot5: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot6: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot7: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot8: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot9: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot10: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot11: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot12: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            slot13: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            clamp: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};