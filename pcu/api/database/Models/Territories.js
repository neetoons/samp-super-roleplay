module.exports = (sequelize, DataTypes) => {
    return sequelize.define("territories",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            gangzone: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            zone: {
                type: DataTypes.STRING(8),
                allowNull: false
            },
            part: {
                type: DataTypes.STRING(8),
                allowNull: false
            },
            name: {
                type: DataTypes.STRING(32),
                allowNull: false
            },
            min_x: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            min_y: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            min_z: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            max_x: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            max_y: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            max_z: {
                type: DataTypes.FLOAT,
                allowNull: false
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};