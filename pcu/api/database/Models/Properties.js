module.exports = (sequelize, DataTypes) => {
    return sequelize.define("properties",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            name: {
                type: DataTypes.STRING(24),
                allowNull: true
            },
            ext_x: {
                type: DataTypes.FLOAT,
                allowNull: true
            },
            ext_y: {
                type: DataTypes.FLOAT,
                allowNull: true
            },
            ext_z: {
                type: DataTypes.FLOAT,
                allowNull: true
            },
            ext_angle: {
                type: DataTypes.FLOAT,
                allowNull: true,
                defaultValue: 0.0
            },
            ext_interior: {
                type: DataTypes.INTEGER,
                allowNull: true,
                defaultValue: 0
            },
            ext_freeze: {
                type: DataTypes.INTEGER,
                allowNull: true,
                defaultValue: 0
            },
            id_interior: {
                type: DataTypes.INTEGER,
                allowNull: true
            },
            price: {
                type: DataTypes.INTEGER,
                allowNull: true,
                defaultValue: 0
            },
            level: {
                type: DataTypes.INTEGER,
                allowNull: true,
                defaultValue: 0
            },
            extra: {
                type: DataTypes.INTEGER,
                allowNull: true,
                defaultValue: 0
            },
            vip_level: {
                type: DataTypes.INTEGER,
                allowNull: true,
                defaultValue: 0
            },
            dis_default_interior: {
                type: DataTypes.INTEGER,
                allowNull: true,
                defaultValue: 0
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};