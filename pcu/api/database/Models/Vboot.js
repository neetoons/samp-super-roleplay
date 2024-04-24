module.exports = (sequelize, DataTypes) => {
    return sequelize.define("vboot",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            type: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            int: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            int_extra: {
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