module.exports = (sequelize, DataTypes) => {
    return sequelize.define("ptoys",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            name: {
                type: DataTypes.STRING(24),
                allowNull: false
            },
            attached: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            modelid: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            bone: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            offset_x: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            offset_y: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            offset_z: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            rot_x: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            rot_y: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            rot_z: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            scale_x: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 1.0
            },
            scale_y: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 1.0
            },
            scale_z: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 1.0
            },
            color1: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            color2: {
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