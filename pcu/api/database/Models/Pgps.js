module.exports = (sequelize, DataTypes) => {
    return sequelize.define("pgps",
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
            x: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            y: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            z: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            world: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            interior: {
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